package src;

import src.modeles.Graphe;
import src.ui.FenetreGraphe;
import java.util.ArrayList;


public class Main {
    public static void main(String[] args) {
        System.out.println("Lancement du Client Algogistique (AWT)...");
        // Lancer l’interface graphique principale ici
        javax.swing.SwingUtilities.invokeLater(() -> {
            FenetreGraphe frame = new FenetreGraphe(1, new Graphe("", new ArrayList<>(), new ArrayList<>()));
            frame.setVisible(true);
        });
    }
}