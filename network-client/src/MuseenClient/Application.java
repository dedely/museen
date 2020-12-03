package MuseenClient;


import java.io.BufferedReader ;
import java.io.InputStreamReader;
import java.io.IOException;

public class Application {
	static boolean On = true;
	static int stateApp = 0;
	static String[] possibleStates = {"begin","authIng","running","ending"};
	
	
	public static void run() {
		
		while(On) {
			
			BufferedReader entree_standard = new BufferedReader (new InputStreamReader ( System.in), 79);
			
			if (stateApp == 0) {	
				
				System.out.println("Entrez l'adresse IP du serveur");
				try{
					String definedIP = entree_standard.readLine();
					ClientTCP.connect(definedIP);
					stateApp = 1;
				}
				catch(IOException e) {
					System.err.println(e.getMessage());
				}
			}
			
			while(stateApp == 1) {
				System.out.println("Entrez votre clé d'authentification :");
				String authKey;
				try {
					authKey = entree_standard.readLine();
					UserManager.login(authKey);
					System.out.println("Bienvenue "+User.getUserID()+" !");
					stateApp = 2;
				} catch (IOException e) {
					System.err.println(e.getMessage());
				}
			}
			
			while(stateApp == 2) {
				System.out.println("Que souhaitez-vous faire ? (info/suggestion/quitter)");
				String codeinput;
				String reponse = "";
				try {
					
					if(!entree_standard.ready()) {
						LocationSimulator.moveonce();
					}
					
					codeinput = entree_standard.readLine();
					
					switch (codeinput.toLowerCase()) {
						case "info" :
							if (User.getLocation() == 1 && User.getLocation() == 2 && User.getLocation() == 3 && User.getLocation() == 4 )
								System.out.println("Vous n'êtes pas devant une oeuvre");
							else {
								reponse = ClientTCP.send("INFO","none");
								System.out.println(reponse);
							}
							
							break;
						
						case "suggestion" :
							reponse = ClientTCP.send("SUGG", "none");
							System.out.println(reponse);
							break;
						
						case "quitter" :
							reponse = ClientTCP.send("EXIT", "none");
							
							System.out.println(reponse);
							
							if(reponse.equals("250")) {
								stateApp = 3;
							}
							else {
								System.out.println("Unexpected logout issue, try again");
							}
								
							
							break;
					}
						
				}
				catch(IOException e) {
					System.err.println(e.getMessage());
				}
			}
			
			while(stateApp == 3) {
				try {
					System.out.println("Deconnexion en cours");
					ClientTCP.disconnect();
					On = false;
				} catch (IOException e) {
					System.err.println(e.getMessage());
				}
				
			}
			
		}
		
		System.exit(1);
	}
	
	public static void main(String args[]) {
		Application.run();
	}

}
