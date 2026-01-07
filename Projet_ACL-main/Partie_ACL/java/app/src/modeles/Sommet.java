package src.modeles;

/**
 * Classe représentant un sommet.
 * Dans ce projet ça représente une ville avec ses coordonnées géographiques
 */
public class Sommet {
    /**
     * Nom de la ville.
     */
    private final String nom;

    /**
     * Latitude de la ville.
     */
    private final double latitude;

    /**
     * Longitude de la ville.
     */
    private final double longitude;

    // Coordonnées pour le dessin (passage monde-écran).

    /**
     * Coordonnée sur l'axe des abscisses.
     */
    private int x;

    /**
     * Coordonnée sur l'axe des ordonnées.
     */
    private int y;


    /**
     * Constructeur.
     * @param nom - Nom de la ville.
     * @param latitude - Latitude de la ville.
     * @param longitude - Longitude de la ville.
     */
    public Sommet(String nom, double latitude, double longitude) {
        this.nom = nom;
        this.latitude = latitude;
        this.longitude = longitude;
    }

    /**
     * Retourne le nom de la ville.
     * @return - Nom de la ville.
     */
    public String getNom() { return nom; }

    /**
     * Retourne la latitude de la ville.
     * @return - Latitude de la ville.
     */
    public double getLatitude() { return latitude; }

    /**
     * Retourne la longitude de la ville.
     * @return - Longitude de la ville.
     */
    public double getLongitude() { return longitude; }

    public int getX() { return x; }
    public int getY() { return y; }

    /**
     * Définit les coordonnées d'écran après calcul de projection.
     * @param x - Coordonnée sur l'axe des abscisses.
     * @param y - Coordonnée sur l'axe des ordonnées.
     */
    public void definirCoordonneesEcran(int x, int y) {
        this.x = x;
        this.y = y;
    }
}
