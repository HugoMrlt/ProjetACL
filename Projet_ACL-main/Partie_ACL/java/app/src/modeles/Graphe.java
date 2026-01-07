package src.modeles;

import java.util.List;

/**
 * Classe représentant un graphe.
 * Dans ce projet, elle représente la carte d'une région.
 */
public class Graphe {
    /**
     * Nom de la fenêtre graphique.
     */
    private final String windowName;

    /**
     * Liste des sommets du graphe.
     */
    private final List<Sommet> sommets;

    /**
     * Liste des arêtes du graphe.
     */
    private final List<Arete> aretes;

    /**
     * Constructeur.
     * @param windowName - Nom de la fenêtre graphique.
     * @param sommets - Liste des sommets du graphe.
     * @param aretes - Liste des arêtes du graphe.
     */
    public Graphe(String windowName, List<Sommet> sommets, List<Arete> aretes) {
        this.windowName = windowName;
        this.sommets = sommets;
        this.aretes = aretes;
    }

    /**
     * Retourne le nom de la fenêtre graphique.
     * @return - Nom de la fenêtre graphique.
     */
    public String getWindowName() { return windowName; }

    /**
     * Retourne la liste des sommets du graphe.
     * @return - Liste des sommets du graphe.
     */
    public List<Sommet> getSommets() { return sommets; }

    /**
     * Retourne la liste des arêtes du graphe.
     * @return - Arêtes du graphe.
     */
    public List<Arete> getAretes() { return aretes; }

    /**
     * Calcule la distance totale (en km) pour un camion ou pour tout le graphe.
     * @param camionId - Identifiant du camion, -1 pour toutes les arêtes.
     * @return distance totale, ou NaN si aucune arête ne correspond.
     */
    public double getDistanceTotale(int camionId) {
        double total = 0.0;
        boolean trouve = false;

        for (Arete arete : aretes) {
            if (camionId != -1 && arete.getCamionId() != camionId) continue;
            total += arete.getPoids();
            trouve = true;
        }

        return trouve ? total : Double.NaN;
    }

    /**
     * Calcule la durée totale estimée (en heures) selon la vitesse des types de routes.
     * @param camionId - Identifiant du camion, -1 pour toutes les arêtes.
     * @return durée en heures, ou NaN si aucune arête ne correspond, ou NEGATIVE_INFINITY si une route a un type inconnu.
     */
    public double getDureeTotale(int camionId) {
        double totalHeures = 0.0;
        boolean trouve = false;

        for (Arete arete : aretes) {
            if (camionId != -1 && arete.getCamionId() != camionId) continue;
            TypeRoute type = TypeRoute.depuisTexte(arete.getTypeRoute());
            double vitesse = type.getVitesse();
            if (vitesse <= 0.0) {
                return Double.NEGATIVE_INFINITY;
            }
            totalHeures += arete.getPoids() / vitesse;
            trouve = true;
        }

        return trouve ? totalHeures : Double.NaN;
    }

    /**
     * Retourne les arêtes avec un type de route inconnu pour un camion donné.
     * @param camionId - Identifiant du camion, -1 pour toutes les arêtes.
     * @return Liste des arêtes problématiques.
     */
    public java.util.ArrayList<Arete> getAretesTypeInconnu(int camionId) {
        java.util.ArrayList<Arete> problemes = new java.util.ArrayList<>();
        for (Arete arete : aretes) {
            if (camionId != -1 && arete.getCamionId() != camionId) continue;
            TypeRoute type = TypeRoute.depuisTexte(arete.getTypeRoute());
            if (type.getVitesse() <= 0.0) {
                problemes.add(arete);
            }
        }
        return problemes;
    }
}