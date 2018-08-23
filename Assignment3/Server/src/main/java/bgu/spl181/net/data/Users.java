package bgu.spl181.net.data;

import java.io.Serializable;
import java.util.List;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class Users implements Serializable {
	
	@SerializedName("users")
	@Expose
	private List<User> users = null;
	
		
		public List<User> getUsers() {
		return users;
		}

		public void setUsers(List<User> users) {
		this.users = users;
		}
}
