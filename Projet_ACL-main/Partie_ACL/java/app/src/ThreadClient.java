package src;

import src.modeles.Graphe;
import src.ui.PanneauControle;

import java.io.*;
import java.net.Socket;

/**
 * Fil d'exécution gérant la communication avec le serveur de calcul.
 * Il envoie la requête et réceptionne les données du graphe résultant.
 */
public class ThreadClient extends Thread {
    /**
     * Chaîne contenant les paramètres de la requête.
     */
    private final String villesRequete;

    /**
     * Constructeur.
     * @param villesRequete - Paramètres de la requête.
     */
    public ThreadClient(String villesRequete) {
        this.villesRequete = villesRequete;
    }

    /**
     * Méthode appelée lors de la réception des données du serveur.
     * Elle sert de "point de contact" (callback) entre le thread de communication et l'interface graphique.
     * Cette méthode est destinée à être redéfinie, par exemple dans {@link PanneauControle}.
     *
     * @param graphe - Le graphe reçu et parsé.
     * @param etiquette - Étiquette descriptive de la requête (ex: nom de la région).
     */
    public void traiterResultat(Graphe graphe, String etiquette) {}

    /**
     * Exécution du thread : connexion, envoi et lecture.
     */
    @Override
    public void run() {
        String hote = "localhost";
        int port = 8080;

        try (Socket socket = new Socket(hote, port);
             PrintWriter fluxSortant = new PrintWriter(socket.getOutputStream(), true);
             // On prépare le BufferedReader ici.
             BufferedReader fluxEntrant = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            System.out.println("Connecté au serveur C++. Envoi de la requête...");

            // Envoyer la liste des villes
            fluxSortant.println(villesRequete);

            // Passer directement le fluxEntrant à Utils.
            Graphe g = UtilitaireCalcul.parseJson(fluxEntrant);

            if (g != null)
                traiterResultat(g, "Région: " + villesRequete);

        } catch (IOException e) {
            System.err.println("Erreur : Impossible de joindre le serveur C++ sur le port " + port);
        }
    }
}