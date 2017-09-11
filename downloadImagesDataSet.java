package com.age.soc.business;
//https://stackoverflow.com/questions/15927014/rotating-an-image-90-degrees-in-java
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;

import javax.imageio.ImageIO;

import com.make.estrutura.exception.TechnicalException;

public class TestJsonShit {

	public static void main(String[] args) throws TechnicalException {

		String arquivoCSV = "C:\\Users\\jessica.mello\\Desktop\\arquivo.csv";
	    BufferedReader br = null;
	    String linha = "";
	    try {

	        br = new BufferedReader(new FileReader(arquivoCSV));
	        int i=0;
	        while ((linha = br.readLine()) != null) {

	            URL url = new URL(linha);
	            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
	            connection.setRequestProperty("User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_5) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.65 Safari/537.31");
	            BufferedImage bufferedImage = ImageIO.read(connection.getInputStream());
	            ImageIO.write(bufferedImage, "jpg", new File("C:\\Users\\jessica.mello\\Desktop\\"+i+".jpg"));
	            i++;
	        }
        } catch (IOException e) {
            e.printStackTrace();
        }
		
	}
	
}
