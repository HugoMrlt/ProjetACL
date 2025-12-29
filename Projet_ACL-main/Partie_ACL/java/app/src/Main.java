package src;

import src.modeles.Graphe;
import src.ui.GraphFrame;
import java.util.ArrayList;


public class Main {
    public static void main(String[] args) {
        System.out.println("Lancement du Client Algogistique (AWT)...");
        // Lancer l’interface graphique principale ici
        javax.swing.SwingUtilities.invokeLater(() -> {
            GraphFrame frame = new GraphFrame(1, new Graphe("", new ArrayList<>(), new ArrayList<>()));
            frame.setVisible(true);
        });
    }
}