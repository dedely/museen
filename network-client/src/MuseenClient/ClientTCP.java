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
	
	
	
	public static void connect (){
        String definedIP ;
        String message ;
        String reponse ;
        
        try {

            // L'IP du serveur sur le réseau
            definedIP = "localhost";
            
            // deuxieme argument : le numero de port que l'on contacte
            socket = new Socket (definedIP, 5000) ;
            flux_sortie = new PrintWriter (socket.getOutputStream (), true) ;
            flux_entree = new BufferedReader (new InputStreamReader (
                                        socket.getInputStream ())) ;
        } 
        catch (UnknownHostException e) {
            System.err.println ("Hote inconnu") ;
            System.exit (1) ;
        }
        catch (IOException e) {
        	System.err.println ("IOException");
        	System.exit(1) ;
        }
        
 	}
	
	public static String send (String message) throws IOException {
		final String separateur = ";";
		String sepMsg[] = message.split(separateur);
		String code = sepMsg[0];
		String datag;
		String reponse;;
        
		switch (code){
			case "INF" :
				datag = code;
				break;
        
			case "PRF" :
				datag = code;
				break;

			case "LOC" :
				datag = code;
				break;
            
			case "LGN" :
				datag = code;
				break;
        	
			default :
				datag = code;
				break;
		}
		
		// On envoit le message associé au code au serveur
        flux_sortie.println (datag);
        
        // On recupere la reponse du serveur
        reponse = flux_entree.readLine() ;

        // On l'affiche (pour l'instant)
        System.out.println ("La reponse du serveur a ete : " + reponse);
        
                
		return reponse;

	}
	
	public static void disconnect() throws IOException {
		socket.close();
	}
	
}
