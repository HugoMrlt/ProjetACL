package src.modeles;

import java.util.List;
import java.util.ArrayList;

/**
 * @brief Classe représentant un graphe côté Client (Visualisation uniquement).
 * * Cette classe est une structure de données passive : elle ne contient aucune
 * logique de calcul conformément à la répartition des tâches (C++ calcul, Java vue).
 * Elle sert à stocker les résultats renvoyés par le serveur pour les afficher.
 */
public class Graphe {
    /** Nom de la fenêtre ou de la région affichée. */
    private final String windowName;

    /** Liste des sommets (villes) à afficher. */
    private final List<Sommet> sommets;

    /** Liste des arêtes (routes/tournées) à dessiner. */
    private final List<Arete> aretes;

    /** Durée totale du parcours calculée par le serveur. */
    private double dureeTotale = 0.0;

    /** Distance totale du parcours calculée par le serveur. */
    private double distanceTotale = 0.0;

    /**
     * @brief Constructeur complet.
     * @param windowName - Nom de la région (ex: "Bretagne").
     * @param sommets - Liste des objets Sommet reçus du JSON.
     * @param aretes - Liste des objets Arete reçus du JSON.
     */
    public Graphe(String windowName, List<Sommet> sommets, List<Arete> aretes) {
        this.windowName = windowName;
        this.sommets = sommets;
        this.aretes = aretes;
    }

    /*--------------- Getters d'affichage ---------------*/

    /** @return - Le nom de la fenêtre. */
    public String getWindowName() { return windowName; }

    /** @return - La liste des sommets du graphe. */
    public List<Sommet> getSommets() { return sommets; }

    /** @return - La liste des arêtes du graphe. */
    public List<Arete> getAretes() { return aretes; }

    /*--------------- Getters / Setters Statistiques ---------------*/

    /** @param duree - La durée totale en heures transmise par le serveur. */
    public void setDureeTotale(double duree) { this.dureeTotale = duree; }

    /** @return - La durée totale pour l'affichage des stats. */
    public double getDureeTotale() { return dureeTotale; }

    /** @param distance - La distance totale en km transmise par le serveur. */
    public void setDistanceTotale(double distance) { this.distanceTotale = distance; }

    /** @return - La distance totale pour l'affichage des stats. */
    public double getDistanceTotale() { return distanceTotale; }

    /**
     * @brief Filtre les arêtes pour l'affichage d'un camion spécifique.
     * * Cette méthode aide l'UI à isoler visuellement la tournée d'un livreur.
     * * @param camionId - L'identifiant du camion (-1 pour afficher tout le graphe).
     * @return List<Arete> - La liste des arêtes appartenant au camion spécifié.
     */
    public List<Arete> getAretesParCamion(int camionId) {
        if (camionId == -1) return aretes;
        List<Arete> resultat = new ArrayList<>();
        for (Arete a : aretes) {
            if (a.getCamionId() == camionId) resultat.add(a);
        }
        return resultat;
    }
}