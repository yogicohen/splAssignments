package bgu.spl181.net.data;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class Movie implements Serializable {

	@SerializedName("id")
	@Expose
	private Integer id;
	@SerializedName("name")
	@Expose
	private String name;
	@SerializedName("price")
	@Expose
	private Integer price;
	@SerializedName("bannedCountries")
	@Expose
	private List<String> bannedCountries = new ArrayList<>();
	@SerializedName("availableAmount")
	@Expose
	private Integer availableAmount;
	@SerializedName("totalAmount")
	@Expose
	private Integer totalAmount;
	
	public Integer getId() {
	return id;
	}
	
	public void setId(Integer id) {
	this.id = id;
	}
	
	public String getName() {
	return name;
	}
	
	public void setName(String name) {
	this.name = name;
	}
	
	public Integer getPrice() {
	return price;
	}
	
	public void setPrice(Integer price) {
	this.price = price;
	}
	
	public List<String> getBannedCountries() {
	return bannedCountries;
	}
	
	public void setBannedCountries(List<String> bannedCountries) {
	this.bannedCountries = bannedCountries;
	}
	
	public Integer getAvailableAmount() {
	return availableAmount;
	}
	
	public void setAvailableAmount(Integer availableAmount) {
	this.availableAmount = availableAmount;
	}
	
	public Integer getTotalAmount() {
	return totalAmount;
	}
	
	public void setTotalAmount(Integer totalAmount) {
	this.totalAmount = totalAmount;
	}
}
