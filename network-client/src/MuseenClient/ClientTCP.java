package MuseenClient;

import java.io.IOException ;
import java.io.BufferedReader ;
import java.io.InputStreamReader ;
import java.io.PrintWriter ;
import java.net.Socket ;
import java.net.UnknownHostException ;

public class ClientTCP {
	static Socket socket;
	static PrintWriter flux_sortie;
	static BufferedReader flux_entree;
	
	
	
	public static void connect (String definedIP){
        String message ;
        String reponse ;
        
        try {
            socket = new Socket (definedIP, 42424) ;
            flux_sortie = new PrintWriter (socket.getOutputStream (), true) ;
            flux_entree = new BufferedReader (new InputStreamReader (
                                        socket.getInputStream ())) ;
        } 
        catch (UnknownHostException e) {
            System.err.println ("Hote inconnu") ;
            System.exit (1) ;
        }
        catch (IOException e) {
        	System.err.println (e.getMessage());
        	System.exit(1) ;
        }
        
 	}
	
	public static String send (String code, String message) throws IOException {
		String datag;
		String reponse;
        
		switch (code){
			case "INFO" :
				datag = code+";"+User.getLocation();
				break;
        
			case "SUGG" :
				datag = code+";"+User.getUserID();
				break;

			case "LOCA" :
				datag = code+";"+User.getUserID()+";"+message;
				break;
            
			case "LOGN" :
				datag = code+";"+message;
				break;
        	
			case "EXIT" :
				datag = code;
				break;
				
			default :
				datag = "none";
				System.err.println ("Unknown Sending Code");
				break;
		}
		
		if(datag != "none") {
			
			//System.out.println("Le datagramme est "+datag);
			// On envoit le message associe au code au serveur
	        flux_sortie.println (datag);
	        
	        // On recupere la reponse du serveur
	        reponse = flux_entree.readLine() ;
	        
	        reponse = filter(reponse);

	        // On l'affiche (pour l'instant)
	        //System.out.println ("La reponse du serveur a ete : " + reponse);
		}
		else {
			reponse = "error";
		}
                
		return reponse;

	}
	
	
	public static String filter(String chaine) {
		
		if(Character.isWhitespace(chaine.charAt(0)) || !(Character.isDigit(chaine.charAt(0)))) {
			int longueur = chaine.length();
			chaine = chaine.substring(1,longueur);
		}
		return chaine;
	}
	
	public static void disconnect() throws IOException {
		socket.close();
	}
	
}
