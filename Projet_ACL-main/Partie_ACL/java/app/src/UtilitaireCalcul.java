package src;

import src.modeles.Arete;
import src.modeles.Graphe;
import src.modeles.Sommet;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import org.json.JSONArray;
import org.json.JSONObject;

/**
 * Classe utilitaire pour les calculs et le parsing des données JSON.
 */
public class UtilitaireCalcul {

    /**
     * Lit un flux de données JSON et le convertit en objet Graphe.
     * @param fluxEntrant - Le flux de lecture des données JSON.
     * @return - Un objet Graphe initialisé ou null en cas d'erreur.
     * @throws IOException - En cas d'erreur de lecture.
     */
    public static Graphe parseJson(BufferedReader fluxEntrant) throws IOException {
        StringBuilder sb = new StringBuilder();
        String ligne;

        // Lecture du flux ligne par ligne pour reconstruire la chaîne JSON complète.
        while ((ligne = fluxEntrant.readLine()) != null)
            sb.append(ligne);

        String json = sb.toString().trim();

        // Vérification sommaire de la validité du format JSON.
        if (json.isEmpty() || !json.startsWith("{"))
            return null;

        // Extraction du nom de la fenêtre et traitement séquentiel des données.
        String nomFenetre = new JSONObject(json).getString("windowName");
        List<Sommet> sommets = lireSommets(json);

        // Calcul automatique du placement des points sur l'écran avant de créer les arêtes.
        convertirCoordonneesVersEcran(sommets);
        List<Arete> aretes = lireAretes(json, sommets);

        return new Graphe(nomFenetre, sommets, aretes);
    }

    /**
     * Calcule la projection des coordonnées géographiques vers les coordonnées d'écran.
     * @param sommets - Liste des sommets à convertir.
     */
    private static void convertirCoordonneesVersEcran(List<Sommet> sommets) {
        // Définition des dimensions de la zone de dessin et des marges de sécurité.
        double largeur = 800; double hauteur = 600; double marge = 50;
        double minLat = Double.MAX_VALUE, maxLat = Double.MIN_VALUE;
        double minLon = Double.MAX_VALUE, maxLon = Double.MIN_VALUE;

        // Recherche des bornes géographiques (min/max) pour cadrer la carte.
        for (Sommet sommet : sommets) {
            minLon = Math.min(minLon, sommet.getLongitude());
            maxLon = Math.max(maxLon, sommet.getLongitude());
            minLat = Math.min(minLat, sommet.getLatitude());
            maxLat = Math.max(maxLat, sommet.getLatitude());
        }

        // Calcul du facteur d'échelle pour que la carte tienne dans la fenêtre tout en gardant ses proportions.
        double facteurEchelle = Math.min((largeur - 2*marge)/(maxLon-minLon), (hauteur - 2*marge)/(maxLat-minLat));

        // Calcul du décalage pour centrer la carte horizontalement et verticalement.
        double decalageX = (largeur - (maxLon-minLon)*facteurEchelle)/2;
        double decalageY = (hauteur - (maxLat-minLat)*facteurEchelle)/2;

        // Application de la transformation mathématique à chaque sommet.
        for (Sommet sommet : sommets) {
            int x = (int) (decalageX + (sommet.getLongitude() - minLon) * facteurEchelle);

            // Inversion de l'axe Y.
            int y = (int) hauteur - (int) (decalageY + (sommet.getLatitude() - minLat) * facteurEchelle);
            sommet.definirCoordonneesEcran(x, y);
        }
    }

    /**
     * Extrait la liste des sommets depuis une chaîne JSON.
     * @param donnees - Chaîne JSON brute.
     * @return - Liste des sommets extraits.
     */
    public static List<Sommet> lireSommets(String donnees) {
        // Récupération du tableau d'objets sous la clé "vertices".
        JSONArray arr = new JSONObject(donnees).getJSONArray("vertices");
        List<Sommet> list = new ArrayList<>();

        for (int i = 0; i < arr.length(); i++) {
            JSONObject o = arr.getJSONObject(i);

            // Création d'un objet Sommet pour chaque entrée du tableau JSON.
            list.add(new Sommet(o.getString("ville"), o.getDouble("latitude"), o.getDouble("longitude")));
        }

        return list;
    }

    /**
     * Extrait la liste des arêtes depuis une chaîne JSON.
     * @param donnees - Chaîne JSON brute.
     * @param sommets - Liste des sommets déjà chargés pour la liaison.
     * @return - Liste des arêtes extraites.
     */
    public static List<Arete> lireAretes(String donnees, List<Sommet> sommets) {
        // Récupération du tableau d'objets sous la clé "edges".
        JSONArray arr = new JSONObject(donnees).getJSONArray("edges");
        List<Arete> list = new ArrayList<>();

        for (int i = 0; i < arr.length(); i++) {
            JSONObject o = arr.getJSONObject(i);
            String v1Name = o.getString("v1"), v2Name = o.getString("v2");
            Sommet v1 = null, v2 = null;

            // Recherche des objets Sommet correspondants aux noms trouvés dans l'arête.
            for (Sommet v : sommets) {
                if (v.getNom().equals(v1Name)) v1 = v;
                if (v.getNom().equals(v2Name)) v2 = v;
            }

            // Si les deux sommets existent, on crée l'arête avec ses propriétés (poids, type, camion).
            if (v1 != null && v2 != null) {
                list.add(new Arete(v1, v2, o.getInt("poids"), o.optString("type", "communale"), o.optInt("camion", 1)));
            }
        }

        return list;
    }
}