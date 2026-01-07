package src.ui;

import src.modeles.Graphe;
import javax.swing.*;
import java.awt.*;

/**
 * Fenêtre principale pour la visualisation du graphe.
 * Cette interface affiche :
 * - Un panneau de contrôle à gauche.
 * - La représentation graphique de la carte à droite.
 */
public class FenetreGraphe extends JFrame {
    /**
     * Constructeur.
     * @param numClient - Numéro du client.
     * @param graphe - Le graphe.
     */
    public FenetreGraphe(int numClient, Graphe graphe) {
        setTitle("Client " + numClient + " : " + graphe.getWindowName());
        setSize(1000, 800);
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setLocationRelativeTo(null);
        PanneauGraphe panneauGraphe = new PanneauGraphe(graphe);
        PanneauControle panneauControle = new PanneauControle(panneauGraphe);
        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, panneauControle, panneauGraphe);
        splitPane.setDividerLocation(250);
        splitPane.setEnabled(false);
        splitPane.setDividerSize(0);
        add(splitPane, BorderLayout.CENTER);
    }
}