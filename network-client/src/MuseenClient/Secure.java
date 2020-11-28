package MuseenClient;

import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;



public class Secure {
	
	public static String securize(String authKey) {
		System.out.println(authKey);
		String hashedKey = encrypt(authKey);
		System.out.println(hashedKey);
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
			e.printStackTrace();
		}
		return password;
		
		
 	}
}
