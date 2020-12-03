package MuseenClient;



public class User {
	private static String userID;
	private String authKey;
	private static int location;
	private boolean loggedIn = false;	
	
	public User(String userID, String authKey, int location) {
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

	
	public static int getLocation() {
		return location;
	}

	public static void setLocation(int newlocation) {
		User.location = newlocation;
	}
	
	
	
	public static String getUserID() {
		return userID;
	}
	
	public void setUserID(String userID) {
		User.userID = userID;
	}
	
	
	
}
