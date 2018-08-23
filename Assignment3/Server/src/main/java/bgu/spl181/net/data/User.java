package bgu.spl181.net.data;


import java.io.Serializable;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class User implements Serializable {
	@SerializedName("username")
	@Expose
	private String username;
	@SerializedName("type")
	@Expose
	private String type = "normal";
	@SerializedName("password")
	@Expose
	private String password;
	@SerializedName("country")
	@Expose
	private String country;
	@SerializedName("movies")
	@Expose
	private List<RentedMovie> movies = new ArrayList<>();
	@SerializedName("balance")
	@Expose
	private Integer balance = 0;

	public String getUsername() {
	return username;
	}

	public void setUsername(String username) {
	this.username = username;
	}

	public String getType() {
	return type;
	}

	public void setType(String type) {
	this.type = type;
	}

	public String getPassword() {
	return password;
	}

	public void setPassword(String password) {
	this.password = password;
	}

	public String getCountry() {
	return country;
	}

	public void setCountry(String country) {
	this.country = country;
	}

	public List<RentedMovie> getMovies() {
	return movies;
	}

	public void setMovies(List<RentedMovie> movies) {
	this.movies = movies;
	}

	public Integer getBalance() {
	return balance;
	}

	public void setBalance(Integer balance) {
	this.balance = balance;
	}
	
	public boolean isAlreadyRentingTheMovie(String movieName) {
		Iterator<RentedMovie> it = this.movies.iterator();
		while(it.hasNext()) {
			if(it.next().getName().equals(movieName))
				return true;
		}
		return false;
	}
	
	public void removeMovieFromRentedList(String movieName) {
		RentedMovie current = null;
		for (int i=0;i<this.movies.size();i++) {
			current = this.movies.get(i);
			if(current.getName().equals(movieName)) {
				this.movies.remove(i);
				return;
			}
		}
	}
	
}
