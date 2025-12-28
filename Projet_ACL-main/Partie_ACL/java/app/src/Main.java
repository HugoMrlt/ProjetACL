package src;
import java.awt.*;

public class Main {
    static void main(String[] args) {
        System.out.println("Lancement du Client Algogistique (AWT)...");

        // Paramètres de la requête
//        String villesRequete = "Lyon, Grenoble, Saint-Étienne, Clermont-Ferrand, Villeurbanne, Annecy, Chambéry, Valence, Vienne, Bourg-en-Bresse, Montélimar, Roanne, Annemasse, Aix-les-Bains, Thonon-les-Bains, Le Puy-en-Velay, Aurillac, Albertville, Villefranche-sur-Saône";
        String villesRequete = "Strasbourg, Forbach, Thionville, Metz, Colmar, Haguenau, Verdun, Nancy";

        // Lancement du thread client
        // On ne passe plus de Socket au constructeur car c'est le thread qui va le créer
        ClientThread client = new ClientThread(1, villesRequete);
        client.start();
    }
}
