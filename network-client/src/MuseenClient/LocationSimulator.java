package MuseenClient;

import java.time.format.DateTimeFormatter;
import java.io.IOException;
import java.time.LocalDateTime;    

public class LocationSimulator {
	static DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
	static LocalDateTime now = LocalDateTime.now();
	static String timestamp = dtf.format(now);
	static int minsec = 10;
	static int maxsec = 600;
	
	public static void moveonce() {
		
		int secondsspent = (int) (minsec + Math.random()*(maxsec-minsec));
		int minutes = secondsspent/60;
		int seconds = secondsspent%60;
		String message;
		String reponse;
		
		String[] timestamparray = timestamp.split("\\s+");
		String time = timestamparray[1];
		String[] splittime = time.split(":");
		int newseconds = (Integer.parseInt(splittime[2])+seconds)%60;
		int newminutes = ((Integer.parseInt(splittime[1])+minutes)+(Integer.parseInt(splittime[2])+seconds)/60)%60;
		int newhours = (Integer.parseInt(splittime[0])+((Integer.parseInt(splittime[1])+minutes)+(Integer.parseInt(splittime[2])+seconds)/60)/60)%24;
		
		String newsecondsstr = String.valueOf(newseconds);
		if (newsecondsstr.length() == 1)
			newsecondsstr = "0"+newsecondsstr;
		
		String newminutesstr = String.valueOf(newminutes);
		if (newminutesstr.length() == 1)
			newminutesstr = "0"+newminutesstr;
		
		timestamparray[1] = String.valueOf(newhours)+":"+newminutesstr+":"+newsecondsstr;
		String newtimestamp = timestamparray[0]+" "+timestamparray[1];
		
		try {
			message = String.valueOf(User.getLocation())+";"+LocationSimulator.timestamp+";"+newtimestamp;
			reponse = ClientTCP.send("LOCA", message);
			if (reponse == "300")
				System.err.println("Invalid Timestamp format");
		}
		catch(IOException e) {
			System.err.println(e.getMessage());
		}
		LocationSimulator.timestamp = newtimestamp;
		
		int newlocation = (int) (1 + Math.random()*(31));
		User.setLocation(newlocation);
		System.out.println("Simulateur : L'utilisateur s'est déplacé à la location de code "+newlocation);
		
	}
}
