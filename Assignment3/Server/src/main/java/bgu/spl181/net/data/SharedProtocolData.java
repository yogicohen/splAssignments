package bgu.spl181.net.data;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonPrimitive;
import com.google.gson.JsonSerializer;
import com.google.gson.stream.JsonReader;


public class SharedProtocolData {
	private Users usersData;
	private Movies moviesData;
	
	private List<String> loggedInUsers = new ArrayList<String>();
	
	private final ReadWriteLock myLock = new ReentrantReadWriteLock();
	private final Lock readLock = myLock.readLock();
	private final Lock writeLock = myLock.writeLock();
	
	private String MoviesFile = "Database/Movies.json";
	private String UsersFile = "Database/Users.json";
	
	public void LoadDataBase() {
		
		Gson gson = new Gson();
		
		try {
			readLock.lock();
			
			JsonReader reader = new JsonReader(new FileReader(MoviesFile));
			moviesData = gson.fromJson(reader,Movies.class); 
			
			JsonReader reader2 = new JsonReader(new FileReader(UsersFile));
			usersData = gson.fromJson(reader2, Users.class);
			
		} catch (FileNotFoundException e) {
		}
		finally {
			readLock.unlock();
		}
		
	}
	
	public void WriteToDataBase() {
		Gson gson = new Gson();
		GsonBuilder builder = new GsonBuilder();
		gson = builder.setPrettyPrinting().create();
		gson = builder.registerTypeAdapter(Integer.class,
				(JsonSerializer<Integer>)(integer, type, jsonSerializationContext) ->
			new JsonPrimitive(integer.toString())).create();
		
		
		try(FileWriter writer = new FileWriter(MoviesFile)) {
			
			writeLock.lock();
			String jsonString = gson.toJson(moviesData);
			writer.write(jsonString);
			
		} catch (IOException e) {
		}
		finally {
			writeLock.unlock();
		}
		
		try(FileWriter writer = new FileWriter(UsersFile)) {
			writeLock.lock();
			String jsonString = gson.toJson(usersData);
			writer.write(jsonString);
			
		} catch (IOException e) {
		}
		finally {
			writeLock.unlock();
		}
	}
	
	

	public List<String> getLoggedInUsers() {
		return loggedInUsers;
	}

	public void setLoggedInUsers(List<String> loggedInUsers) {
		this.loggedInUsers = loggedInUsers;
	}
	
	public User getUserByName(String username) {
		Iterator<User> it = usersData.getUsers().iterator();
		User current = null;
		while(it.hasNext()) {
			current=it.next();
			if (current.getUsername().equals(username))
				return current;
		}
		return null;
	}
	
	
	public Movie getMovieByName(String movieName) {
		Iterator<Movie> it = moviesData.getMovies().iterator();
		Movie current = null;
		while(it.hasNext()) {
			current=it.next();
			if (current.getName().equals(movieName))
				return current;
		}
		return null;
	}
	
	
	public List<String> getExistingMovieNames(){
		List<String> toReturn = new ArrayList<String>();
		Iterator<Movie> it=moviesData.getMovies().iterator();
		while(it.hasNext())
			toReturn.add(it.next().getName());
		
		return toReturn;
	}
	
	public Integer getHighestMovieId() {
		Integer toReturn=0;
		for (int i=0;i<moviesData.getMovies().size();i++) {
			if (moviesData.getMovies().get(i).getId()>toReturn)
				toReturn=moviesData.getMovies().get(i).getId();
		}
		return toReturn;
	}
	
	public List<String> getExistingUserNames(){
		List<String> toReturn = new ArrayList<String>();
		Iterator<User> it=usersData.getUsers().iterator();
		while(it.hasNext())
			toReturn.add(it.next().getUsername());
		
		return toReturn;
	} 

	public List<Movie> getMovies() {
		// TODO Auto-generated method stub
		return moviesData.getMovies();
	}

	public List<User> getUsers() {
		// TODO Auto-generated method stub
		return usersData.getUsers();
	}

}
