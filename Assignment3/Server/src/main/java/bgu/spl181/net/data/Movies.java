package bgu.spl181.net.data;

import java.io.Serializable;
import java.util.List;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class Movies implements Serializable {
	@SerializedName("movies")
	@Expose
	private List<Movie> movies = null;
	
	public List<Movie> getMovies() {
		return movies;
		}

		public void setMovies(List<Movie> movies) {
		this.movies = movies;
		}
}
