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
					if(UserManager.login(authKey)){
						System.out.println("Bienvenue "+User.getUserID()+" !");
						stateApp = 2;
					}
					else {
						System.out.println("Authentification refusée");
					}
				} catch (IOException e) {
					System.err.println(e.getMessage());
				}
			}
			
			while(stateApp == 2) {
				System.out.println("Que souhaitez-vous faire ? (info/suggestion/quitter)");
				String codeinput;
				String reponse = "";
				String[] arrayreponse;
				try {
					
					if(!entree_standard.ready()) {
						LocationSimulator.moveonce();
					}
					
					codeinput = entree_standard.readLine();
					
					switch (codeinput.toLowerCase()) {
						case "info" :
							if (User.getLocation() == 1 || User.getLocation() == 2 || User.getLocation() == 3 || User.getLocation() == 4 )
								System.out.println("Vous n'êtes pas devant une oeuvre");
							else {
								reponse = ClientTCP.send("INFO","none");
								arrayreponse = reponse.split(";");	
								
								if(arrayreponse[0].equals("310")) {
									displayinfo(arrayreponse);
									
								}
								else {
									System.out.println(reponse);
								}
							}
							
							break;
						
						case "suggestion" :
							reponse = ClientTCP.send("SUGG", "none");
							System.out.println(reponse);
							
							break;
						
						case "quitter" :
							reponse = ClientTCP.send("EXIT", "none");
							
							//System.out.println(reponse);
							
							if(reponse.equals("250")) {
								stateApp = 3;
							}
							else {
								System.out.println("Erreur de déconnexion. Veuillez réessayer.");
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
					System.out.println("Déconnexion en cours");
					ClientTCP.disconnect();
					stateApp = 0;
					On = false;
				} catch (IOException e) {
					System.err.println(e.getMessage());
				}
				System.out.println("Bye !");
			}
			
		}
		
	}
	
	public static void displayinfo(String[] elements) {
		
		String auteur;
		String determinant;
		
		if(elements[5].equals("?")) {
			auteur = elements[4];
		}
		else {
			auteur = elements[4]+" "+elements[5];
		}
		
		if(elements[2].equals("Tableau")) {
			determinant = "un ";
		}
		else {
			determinant = "une ";
		}
		
		String information = elements[1]+" est "+determinant+elements[2]+" de "+elements[3]+" par "+auteur+" qui est un "+elements[8]+" dont la date de naissance est "+elements[6]+" et la date de décès est "+elements[7]+".";
		String informationsuite = "Il s'agit d'une oeuvre du mouvement "+elements[9]+" qui se trouve à cet emplacement: "+elements[10];
		System.out.println(information);
		System.out.println(informationsuite);
		
	}
	
	public static void main(String args[]) {
		Application.run();
	}

}
