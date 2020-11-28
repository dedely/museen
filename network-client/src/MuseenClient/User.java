package MuseenClient;



public class User {
	private String userID;
	private String authKey;
	private String location;
	private boolean loggedIn = false;	
	
	public User(String userID, String authKey, String location) {
		this.userID = userID;
		this.authKey = authKey;
		this.location = location;
	}

	/*
	*/

	
	public String getAuthKey() {
		return authKey;
	}

	public void setAuthKey(String authKey) {
		this.authKey = authKey;
	}

	
	public boolean isLoggedin() {
		return loggedIn;
	}

	public void setLoggedin(boolean loggedin) {
		this.loggedIn = loggedin;
	}

	
	public String getLocation() {
		return location;
	}

	public void setLocation(String newlocation) {
		this.location = newlocation;
	}
	
	
	
	public String getUserID() {
		return userID;
	}
	
	public void setUserID(String userID) {
		this.userID = userID;
	}
	
	
	
}
