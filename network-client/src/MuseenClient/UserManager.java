package MuseenClient;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class UserManager {
	
	int unsuccessful = 0;
	
	public static String securize(String authKey) {
		//System.out.println(authKey);
		String hashedKey = encrypt(authKey);
		//System.out.println(hashedKey);
		return hashedKey;
		
	}
	
	private static String encrypt(String password) {
		MessageDigest md;
		try {
			//On sélectionne le bon MessageDigest (c'est un factory pattern)
			md = MessageDigest.getInstance("SHA-256");
			
			//On génère le mot de passe hashé
			byte[] hashedPassword =  md.digest(password.getBytes(StandardCharsets.UTF_8));
			
			//On convertit le mot de passe hashé en string
			StringBuilder sb = new StringBuilder();
			for(byte b : hashedPassword) {
				sb.append(String.format("%02x", b));	
			}
			password = sb.toString();

		} catch(NoSuchAlgorithmException e) {
			System.err.println(e.getMessage());
		}
		return password;
	}
	
	public static boolean login(String authKey) {
		boolean connected = false;
		String answer;
		String hashedKey = UserManager.securize(authKey);
		String code = "LOGN";
			
		try {
			answer = ClientTCP.send(code, hashedKey);
			String[] answerarray = answer.split(";");
			if(answerarray[0].equals("110")) {
				User user = new User(answerarray[1], authKey, 1);
				connected = true;
			}
			else if (answerarray[0].equals("140")) {
				System.out.println("Le serveur n'est pas prêt. Veuillez réessayez.");
			}
			else {
				System.out.println("Authentification refusée. Vérifiez votre clé ou contactez le musée.");
			}
		} catch (IOException e) {
			System.out.println(e.getMessage());
		}
		return connected;
	}
	
	
}
