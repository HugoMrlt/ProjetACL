package src.ui;

import src.modeles.Graphe;
import javax.swing.*;
import java.awt.*;

public class GraphFrame extends JFrame {
    public GraphFrame(int clientNb, Graphe data) {
        setTitle("Client " + clientNb + " : " + data.getWindowName());
        setSize(1000, 800);
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setLocationRelativeTo(null);
        // Création du panneau graphique
        GraphPanel graphPanel = new GraphPanel(data);
        // Passage de la référence du panneau graphique au panneau de contrôle
        ControlPanel controlPanel = new ControlPanel(graphPanel);
        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, controlPanel, graphPanel);
        splitPane.setDividerLocation(250); // Largeur du panneau de contrôle
            splitPane.setEnabled(false); // Désactive le redimensionnement
            splitPane.setDividerSize(0); // Rend le séparateur invisible
        add(splitPane, BorderLayout.CENTER);
    }
}