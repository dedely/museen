package MuseenClient;



public class ClientUser {
	private String UID;
	private String authKey;
	private String Location;
	private boolean loggedin = false;	
	
	public void loging(String authKey) {
		this.authKey = authKey;
		String answer;
		String hashedKey = Secure.securize(authKey);
		String code = "LI";
		
		//try {
			
			answer = ClientTCP.send(code+";"+hashedKey);
			//Vérification, etc.
			this.loggedin = true;
			
		//}catch(/*TBD*/) {}
				
	}
	
	public void setLocation(String newlocation) {
		this.Location = newlocation;
	}
	
	public void logout(){
		this.loggedin = false;
	}
	
	public String getUID() {
		return UID;
	}
	
	public boolean isLoggedIn() {
		return loggedin;
	}
	
	
	
	
}
