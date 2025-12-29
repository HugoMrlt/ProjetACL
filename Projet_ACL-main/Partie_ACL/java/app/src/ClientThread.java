package src;

import src.modeles.Graphe;
import src.ui.GraphFrame;

import java.awt.*;
import java.io.*;
import java.net.Socket;

public class ClientThread extends Thread {
    private final int clientNb;
    private final String villesRequete;

    public ClientThread(int clientNb, String villesRequete) {
        this.clientNb = clientNb;
        this.villesRequete = villesRequete;
    }

    public void onResult(Graphe data, String label) {}

    @Override
    public void run() {
        String host = "localhost";
        int port = 8080;

        try (Socket socket = new Socket(host, port);
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
             // On prépare le BufferedReader ici
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            System.out.println("Connecté au serveur C++. Envoi de la requête...");

            // 1. Envoyer la liste des villes
            out.println(villesRequete);

            // 2. Passer directement le flux 'in' (BufferedReader) à Utils
            // Note : Utils.parseJson(in) doit lire les lignes lui-même
            Graphe data = Utils.parseJson(in);

            if (data != null) {
                // Notifier le parent (ControlPanel) si surchargé
                onResult(data, "Région: " + villesRequete);
            }

        } catch (IOException e) {
            System.err.println("Erreur : Impossible de joindre le serveur C++ sur le port " + port);
            e.printStackTrace();
        }
    }

    private void startWindow(int clientNb, Graphe data) {
        // Ici, tu appelles ta classe de fenêtre AWT (ex: GraphFrame)
        // new GraphFrame(clientNb, data).setVisible(true);
        System.out.println("Affichage de la solution pour : " + data.getWindowName());
    }
}