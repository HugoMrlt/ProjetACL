package src.ui;

import src.modeles.Graphe;
import javax.swing.*;

public class GraphFrame extends JFrame {
    public GraphFrame(int clientNb, Graphe data) {
        setTitle("Client " + clientNb + " : " + data.getWindowName());
        setSize(1000, 800);
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setLocationRelativeTo(null);
        add(new GraphPanel(data));
    }
}