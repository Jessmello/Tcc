package teste;
//https://stackoverflow.com/questions/15927014/rotating-an-image-90-degrees-in-java
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;

import javax.imageio.ImageIO;

public class teste {

	public static void main(String[] args) {

		String arquivoCSV = "C:\\Users\\Jess\\Desktop\\arquivo.csv";
	    BufferedReader br = null;
	    String linha = "";
	    try {
	        br = new BufferedReader(new FileReader(arquivoCSV));
	        int i=0;
	        while ((linha = br.readLine()) != null && i < 1918) {
	        	i++;
	        }
	        while ((linha = br.readLine()) != null) {
	        	
	        	System.out.println("Lendo linha: " + i);
	            URL url = new URL(linha);
	            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
	            connection.setRequestProperty("User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_5) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.65 Safari/537.31");
	            try {
		            BufferedImage bufferedImage = ImageIO.read(connection.getInputStream());
		            if(bufferedImage != null){
		            	ImageIO.write(bufferedImage, "jpg", new File("C:\\Users\\Jess\\Desktop\\imageNet\\"+i+".jpg"));
		            }
		            
	            } catch (IOException e) {
	                //e.printStackTrace();
	            }
	            i++;
	        }
        } catch (IOException e) {
            e.printStackTrace();
        }
		
	}
	
}
