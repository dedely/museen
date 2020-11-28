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
				ClientTCP.connect();
				stateApp = 1;
			}
			
			while(stateApp == 1) {
				System.out.println("Entrez votre clé d'authentification : \n");
				String authKey;
				try {
					authKey = entree_standard.readLine();
					UserManager.login(authKey);
					stateApp = 2;
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					//stateApp = 0; ??
				}
			}
			
			while(stateApp == 2) {
				stateApp = 3;
			}
			
			while(stateApp == 3) {
				try {
					ClientTCP.disconnect();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				On = false;
			}
			
		}
	}
	
	public static void main(String args[]) {
		Application.run();
	}

}
