package bgu.spl181.net.impl;

import java.util.ArrayList;
import bgu.spl181.net.api.bidi.BidiMessagingProtocol;
import bgu.spl181.net.api.bidi.Connections;
import bgu.spl181.net.data.SharedProtocolData;
import bgu.spl181.net.data.User;


public class UserServiceTextBasedProtocol implements BidiMessagingProtocol<String>{
	
	protected int connectionId=0;
	protected Connections<String> myConnections;
	protected boolean shouldTerminate = false;
	protected SharedProtocolData data =null;
	protected static ArrayList<Integer> loggedInUsersConnectionId=new ArrayList<>();
	protected String UserNameWhenLoggedIn="";
	
	public UserServiceTextBasedProtocol(SharedProtocolData data) {
		this.data = data;
	}
	
	@Override
	public void start(int connectionId, Connections<String> connections) {
		// TODO Auto-generated method stub
		this.connectionId=connectionId;
		this.myConnections=connections;
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
		
		if (command.equals("SIGNOUT")|command.equals("signout")) {
			handleSignout(this.UserNameWhenLoggedIn);
			return;
		}
		
		if(message.indexOf(' ')>0)
			message = message.substring(message.indexOf(' '));
		
		String username = "";
		String password = "";
		String datablock = "";
		String requestName = "";
		String parameters="";
		
		if (command.equals("REQUEST") | command.equals("request")) {
			message=message.trim();
			
			if(!message.isEmpty())
				requestName=message.substring(0, message.indexOf(' '));
				
			if(!message.isEmpty())
				parameters=message.substring(message.indexOf(' '),message.lastIndexOf(' '));
				
			handleRequest(requestName,parameters);
		}
		
		else {
			
		message=message.trim();
		if(!message.isEmpty()) {
			if(message.indexOf(' ')>0) {
				username=message.substring(0, message.indexOf(' '));
				message=message.substring(message.indexOf(username)+username.length());
			}
			else {
				username=message;
				message="";
			}
		}
		
		message=message.trim();
		if(message.indexOf(' ')>0) {
			password=message.substring(0, message.indexOf(' '));
			message=message.substring(message.indexOf(password)+password.length());
		}
		else {
			password = message;
			message="";
		}
		
		message=message.trim();
		if (!message.isEmpty())
			datablock = message.substring(0);
		
		
		if (command.equals("REGISTER") | command.equals("register"))
			handleRegister(username,password,datablock);
		
		if (command.equals("LOGIN")|command.equals("login"))
			handleLogin(username,password);
	}
}

	@Override
	public boolean shouldTerminate() {
		// TODO Auto-generated method stub
		return shouldTerminate;
	}
	
	
	
	protected void handleRegister(String username, String password, String datablock) {
		if(!this.UserNameWhenLoggedIn.isEmpty()) {
			myConnections.send(this.connectionId, "ERROR registration failed");
			return;
		}
		//Missing info
		if(username.isEmpty() | password.isEmpty()) {
			myConnections.send(this.connectionId, "ERROR registration failed");
			return;
		}
		//client performing the register call is already logged in
		if(data.getLoggedInUsers().contains(username)) {
			myConnections.send(this.connectionId, "ERROR registration failed");
			return;
		}
		//username already registered in the System
		if (data.getUserByName(username)!=null) {
			myConnections.send(this.connectionId, "ERROR registration failed");
			return;
		}
			
		//datablock does not fit requirements
		if(!datablock.isEmpty()) {
			if(!datablock.contains("country")) {
				myConnections.send(this.connectionId, "ERROR registration failed");
				return;
			}
		}
		
		User toAdd = new User();
		toAdd.setUsername(username);
		toAdd.setPassword(password);
		if(!datablock.isEmpty()) {
			datablock = datablock.substring(datablock.indexOf('"')+1,datablock.lastIndexOf('"'));
			toAdd.setCountry(datablock);
		}
		
		myConnections.send(this.connectionId, "ACK registration succeeded");
		data.getUsers().add(toAdd);
		data.WriteToDataBase();
	}
	
	
	
	protected void handleLogin(String username,String password) {
		if(!this.UserNameWhenLoggedIn.isEmpty()) {
			myConnections.send(this.connectionId, "ERROR login failed");
			return;
		}
		//user doesnt exist in database
		if(!data.getExistingUserNames().contains(username)) {
			myConnections.send(this.connectionId, "ERROR login failed");
			return;
		}
		//already logged in
		if(data.getLoggedInUsers().contains(username)) {
			myConnections.send(this.connectionId, "ERROR login failed");
			return;
		}
		//username and password doesnt match
		if (!data.getUserByName(username).getPassword().equals(password)) {
			myConnections.send(this.connectionId, "ERROR login failed");
			return;
		}
		
		data.getLoggedInUsers().add(username);
		loggedInUsersConnectionId.add(this.connectionId);
		this.UserNameWhenLoggedIn=username;
		myConnections.send(this.connectionId, "ACK login succeeded");
	}
	
	
	
	protected void handleSignout(String username) {
		//client not logged in
		if(!data.getLoggedInUsers().contains(username)) {
			myConnections.send(this.connectionId, "ERROR signout failed");
			return;
		}
		
		data.getLoggedInUsers().remove(username);
		loggedInUsersConnectionId.remove((Object)this.connectionId);
		this.UserNameWhenLoggedIn="";
		myConnections.send(this.connectionId, "ACK signout succeeded");
		this.shouldTerminate = true;
		this.myConnections.disconnect(this.connectionId);
		
	}
	
	protected void handleRequest(String requestName, String parameters) {
		if (!data.getLoggedInUsers().contains(this.UserNameWhenLoggedIn)) {
			myConnections.send(this.connectionId, "ERROR request "+requestName+" failed");
			return;
		}
		else return;
	}
	
	protected void BROADCAST(String msg) {
		for (int i=0;i<loggedInUsersConnectionId.size();i++)
			myConnections.send(loggedInUsersConnectionId.get(i), msg);
	}
	
}
