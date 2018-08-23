package bgu.spl181.net.impl;

import java.util.ArrayList;
import java.util.List;

import bgu.spl181.net.data.Movie;
import bgu.spl181.net.data.RentedMovie;
import bgu.spl181.net.data.SharedProtocolData;
import bgu.spl181.net.data.User;

public class MRSProtocol extends UserServiceTextBasedProtocol {
	
	
	public MRSProtocol (SharedProtocolData data) {
		super(data);
	}
	
	@Override
	public void process(String message) {
		// TODO Auto-generated method stub
		
		data.LoadDataBase();
		
		message=message.trim();
		String command="";
		if(message.indexOf(' ')>0)
			command=message.substring(0,message.indexOf(' '));
		else command=message.trim();
		
		//means that command is either register/login/signout
		if(!command.equals("REQUEST") & !command.equals("request")) {
			super.process(message);
			return;
		}
		
		message=message.trim();
		message = message.substring(message.indexOf(' '));
		
		if(message.contains("balance info"))
			handleRequestBalanceInfo();
		
		
		
		else if(message.contains("balance add")) {
			message=message.substring(message.indexOf("add")+3);
			
			message=message.trim();
			
			if(!message.isEmpty()) {
				Integer balanceToAdd = Integer.valueOf(message);
				handleRequestBalanceAdd(balanceToAdd);
			}
			else handleRequestBalanceAdd(0);
		}
		
		
		
		else if (message.contains("info")) {
			message=message.substring(message.indexOf("info")+4);
			message=message.trim();
			String movieName="";
			if(!message.isEmpty()) {
				movieName = message.substring(message.indexOf('"')+1,message.lastIndexOf('"'));
				handleRequestInfo(movieName);
			}
			else handleRequestInfo("");
			
		}
		
		else if (message.contains("rent")) {
			message=message.substring(message.indexOf("rent")+4);
			message=message.trim();
			String movieName= message.substring(message.indexOf('"')+1,message.lastIndexOf('"'));
			handleRequestRent(movieName);
		}
		
		else if (message.contains("return")) {
			message=message.substring(message.indexOf("return")+6);
			message=message.trim();
			String movieName= message.substring(message.indexOf('"')+1,message.lastIndexOf('"'));
			handleRequestReturn(movieName);
		}
		
		else if (message.contains("addmovie")) {
			message=message.substring(message.indexOf("addmovie")+8);
			message=message.trim();
			String movieName = message.substring(message.indexOf('"')+1);
			movieName = movieName.substring(0,movieName.indexOf('"'));
			
			message=message.substring(message.indexOf(movieName)+movieName.length()+2);
			message=message.trim();
			
			Integer amount = Integer.valueOf(message.substring(0,message.indexOf(' ')));
			message=message.substring(message.indexOf(' '));
			message=message.trim();
			Integer price=0;
			if (message.indexOf(' ')>0)
				price = Integer.valueOf(message.substring(0,message.indexOf(' ')));
			else price =Integer.valueOf(message);
			
			if(message.indexOf(' ')>0)
				message=message.substring(message.indexOf(' '));
			else message="";
			
			message=message.trim();
			if(!message.isEmpty())
				handleRequestAddMovie(movieName,amount,price,message);
			else handleRequestAddMovie(movieName,amount,price,"");
		}
		
		else if (message.contains("remmovie")) {
			message=message.substring(message.indexOf("remmovie")+8);
			message=message.trim();
			String movieName = message.substring(message.indexOf('"')+1,message.lastIndexOf('"'));
			handleRequestRemove(movieName);
		}
		
		else if (message.contains("changeprice")) {
			message=message.substring(message.indexOf("changeprice")+11);
			message=message.trim();
			String movieName= message.substring(message.indexOf('"')+1,message.lastIndexOf('"'));
			message=message.substring(message.lastIndexOf('"')+1);
			message=message.trim();
			Integer newPrice = Integer.valueOf(message);
			handleRequestChangePrice(movieName,newPrice);
		}
		
	}
	
	private void handleRequestBalanceInfo() {
		if(!data.getLoggedInUsers().contains(this.UserNameWhenLoggedIn)) {
			myConnections.send(this.connectionId, "ERROR request balance info failed");
			return;
		}
		else {
			User toGetBalanceInfoFrom = data.getUserByName(this.UserNameWhenLoggedIn);
			Integer balance = toGetBalanceInfoFrom.getBalance();
			myConnections.send(this.connectionId, "ACK balance "+balance.toString());
			return;
		}
	}
	
	private void handleRequestBalanceAdd(Integer toAdd) {
		if(!data.getLoggedInUsers().contains(this.UserNameWhenLoggedIn) || toAdd==0) {
			myConnections.send(this.connectionId, "ERROR request balance add failed");
			return;
		}
		User toChangeBalance = data.getUserByName(this.UserNameWhenLoggedIn);
		toChangeBalance.setBalance(toChangeBalance.getBalance()+toAdd);
		data.WriteToDataBase();
		myConnections.send(this.connectionId, "ACK balance "+toChangeBalance.getBalance().toString()
				+" added "+toAdd.toString());
	}
	
	
	private void handleRequestInfo(String movieName) {
		if(!data.getLoggedInUsers().contains(this.UserNameWhenLoggedIn)) {
			myConnections.send(this.connectionId, "ERROR request info failed");
			return;
		}
		if (!movieName.isEmpty()) {
			if(!data.getExistingMovieNames().contains(movieName)) {
				myConnections.send(this.connectionId, "ERROR request info failed");
				return;
			}
			//movieName isn't empty and exists on the system
			else {
				Movie toGetInfoFrom = data.getMovieByName(movieName);
				String infoToSend="";
				infoToSend +='"'+toGetInfoFrom.getName()+'"'+" "+toGetInfoFrom.getAvailableAmount()+
						" "+toGetInfoFrom.getPrice();
				if(toGetInfoFrom.getBannedCountries()!=null)
					for(int i=0;i<toGetInfoFrom.getBannedCountries().size();i++)
						infoToSend+=" "+'"'+toGetInfoFrom.getBannedCountries().get(i)+'"';
				
				myConnections.send(this.connectionId, "ACK info "+infoToSend);
				return;
			}
		}
		//there was no given name, need to return all movie names
		else {
			List<String> moviesToSend = data.getExistingMovieNames();
			String infoToSend="";
			for(int i=0;i<moviesToSend.size();i++) {
				infoToSend+='"'+moviesToSend.get(i)+'"';
				if(i+1<moviesToSend.size())
					infoToSend+=" ";
			}
			myConnections.send(this.connectionId, "ACK info "+infoToSend);
			return;
		}
		
	}
	
	private void handleRequestRent(String movieName) {
		if(!data.getLoggedInUsers().contains(UserNameWhenLoggedIn)) {
			myConnections.send(this.connectionId, "ERROR request rent failed");
			return;
		}
		
		Movie wantedMovie = data.getMovieByName(movieName);
		User userRequestRenting = data.getUserByName(UserNameWhenLoggedIn);
		
		if(wantedMovie==null || wantedMovie.getAvailableAmount()==0) {
			myConnections.send(this.connectionId, "ERROR request rent failed");
			return;
		}
		
		if (userRequestRenting.getBalance()<wantedMovie.getPrice()) {
			myConnections.send(this.connectionId, "ERROR request rent failed");
			return;
		}
		
		if(wantedMovie.getBannedCountries().contains(userRequestRenting.getCountry())) {
			myConnections.send(this.connectionId, "ERROR request rent failed");
			return;
		}
		
		if(userRequestRenting.isAlreadyRentingTheMovie(movieName)) {
			myConnections.send(this.connectionId, "ERROR request rent failed");
			return;
		}
		
		wantedMovie.setAvailableAmount(wantedMovie.getAvailableAmount()-1);
		userRequestRenting.setBalance(userRequestRenting.getBalance()-wantedMovie.getPrice());
		RentedMovie toAdd = new RentedMovie();
		toAdd.setId(wantedMovie.getId());
		toAdd.setName(wantedMovie.getName());
		userRequestRenting.getMovies().add(toAdd);
		
		data.WriteToDataBase();
		
		myConnections.send(this.connectionId, "ACK rent "+'"'+movieName+'"'+" success");
		super.BROADCAST("BROADCAST movie "+'"'+movieName+'"'+" "+
				wantedMovie.getAvailableAmount()+" "+wantedMovie.getPrice());
		return;
		
	}
	
	private void handleRequestReturn(String movieName) {
		if(!data.getLoggedInUsers().contains(UserNameWhenLoggedIn)) {
			myConnections.send(this.connectionId, "ERROR request return failed");
			return;
		}
		
		User userRequestReturning = data.getUserByName(UserNameWhenLoggedIn);
		Movie toReturn = data.getMovieByName(movieName);
		
		if(!userRequestReturning.isAlreadyRentingTheMovie(movieName)) {
			myConnections.send(this.connectionId, "ERROR request return failed");
			return;
		}
		
		if(toReturn==null) {
			myConnections.send(this.connectionId, "ERROR request return failed");
			return;
		}
		
		userRequestReturning.removeMovieFromRentedList(movieName);
		
		toReturn.setAvailableAmount(toReturn.getAvailableAmount()+1);
		
		data.WriteToDataBase();
		
		myConnections.send(this.connectionId, "ACK return "+'"'+movieName+'"'+" success");
		super.BROADCAST("BROADCAST movie "+'"'+movieName+'"'+" "+
				toReturn.getAvailableAmount()+" "+toReturn.getPrice());
		return;
	}
	
	private void handleRequestAddMovie(String name,Integer amount,Integer price,String bannedCountries) {
		if (!data.getLoggedInUsers().contains(this.UserNameWhenLoggedIn)) {
			myConnections.send(this.connectionId, "ERROR request addmovie failed");
			return;
		}
		User myUser = data.getUserByName(UserNameWhenLoggedIn);
		
		if(!myUser.getType().equals("admin")) {
			myConnections.send(this.connectionId, "ERROR request addmovie failed");
			return;
		}
		
		if(data.getExistingMovieNames().contains(name) | amount<=0 | price<=0) {
			myConnections.send(this.connectionId, "ERROR request addmovie failed");
			return;
		}
		ArrayList<String> bannedCountriesList = new ArrayList<>();
		
		if(!bannedCountries.isEmpty()) {
			while(!bannedCountries.isEmpty()) {
				bannedCountries=bannedCountries.trim();
				String nextCountry ="";
				if(bannedCountries.indexOf(' ')>0) {
					nextCountry=bannedCountries.substring(bannedCountries.indexOf('"')+1, bannedCountries.indexOf(' ')-1);
					bannedCountries=bannedCountries.substring(bannedCountries.indexOf(' ')+1);
					bannedCountries=bannedCountries.trim();
				}
				else {
					nextCountry=bannedCountries.substring(bannedCountries.indexOf('"')+1,bannedCountries.length()-1);
					bannedCountries="";
				}
				bannedCountriesList.add(nextCountry);
			}
		}
		
		Movie toAdd = new Movie();
		toAdd.setName(name);
		toAdd.setTotalAmount(amount);
		toAdd.setAvailableAmount(amount);
		toAdd.setId(data.getHighestMovieId()+1);
		toAdd.setPrice(price);
		toAdd.setBannedCountries(bannedCountriesList);
		data.getMovies().add(toAdd);
		
		data.WriteToDataBase();
		
		myConnections.send(this.connectionId, "ACK addmovie "+'"'+name+'"'+" success");
		super.BROADCAST("BROADCAST movie "+'"'+name+'"'+" "+toAdd.getAvailableAmount()+" "+toAdd.getPrice());
		return;
	}
	
	private void handleRequestRemove(String movieName) {
		if (!data.getLoggedInUsers().contains(this.UserNameWhenLoggedIn)) {
			myConnections.send(this.connectionId, "ERROR request remmovie failed");
			return;
		}
		User myUser = data.getUserByName(UserNameWhenLoggedIn);
		
		if(!myUser.getType().equals("admin")) {
			myConnections.send(this.connectionId, "ERROR request remmovie failed");
			return;
		}
		
		Movie toDelete = data.getMovieByName(movieName);
		
		if (toDelete==null) {
			myConnections.send(this.connectionId, "ERROR request remmovie failed");
			return;
		}
		if(toDelete.getAvailableAmount()!=toDelete.getTotalAmount()) {
			myConnections.send(this.connectionId, "ERROR request remmovie failed");
			return;
		}
		
		data.getMovies().remove(toDelete);
		data.WriteToDataBase();
		
		myConnections.send(this.connectionId, "ACK remmovie "+'"'+movieName+'"'+" success");
		super.BROADCAST("BROADCAST movie "+'"'+movieName+'"'+" removed");
		return;
	}
	
	private void handleRequestChangePrice(String movieName,Integer newPrice) {
		if (!data.getLoggedInUsers().contains(this.UserNameWhenLoggedIn)) {
			myConnections.send(this.connectionId, "ERROR request changeprice failed");
			return;
		}
		
		User myUser = data.getUserByName(UserNameWhenLoggedIn);
		
		if(!myUser.getType().equals("admin")) {
			myConnections.send(this.connectionId, "ERROR request changeprice failed");
			return;
		}

		Movie toChange = data.getMovieByName(movieName);
		
		if (toChange==null) {
			myConnections.send(this.connectionId, "ERROR request changeprice failed");
			return;
		}
		
		if(newPrice<=0) {
			myConnections.send(this.connectionId, "ERROR request changeprice failed");
			return;
		}
		
		toChange.setPrice(newPrice);
		data.WriteToDataBase();
		
		myConnections.send(this.connectionId, "ACK changeprice "+'"'+movieName+'"'+" success");
		super.BROADCAST("BROADCAST movie "+'"'+movieName+'"'+" "+toChange.getAvailableAmount()
		+" "+toChange.getPrice());
		return;
	}
	
	
}
