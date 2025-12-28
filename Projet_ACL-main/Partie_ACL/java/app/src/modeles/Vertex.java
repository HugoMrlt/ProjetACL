package src.modeles;

/**
 * Représente une ville (sommet) avec ses coordonnées géographiques
 * et ses coordonnées de rendu écran[cite: 11, 118].
 */
public class Vertex {
    private final String name;
    private final double latitude;  // Remplace realY pour la clarté [cite: 11]
    private final double longitude; // Remplace realX pour la clarté [cite: 11]

    // Coordonnées pour le dessin AWT (passage monde-écran)
    private int x;
    private int y;

    public Vertex(String name, double latitude, double longitude) {
        this.name = name;
        this.latitude = latitude;
        this.longitude = longitude;
    }

    public String getName() { return name; }
    public double getLatitude() { return latitude; }
    public double getLongitude() { return longitude; }

    public int getX() { return x; }
    public int getY() { return y; }

    /**
     * Définit les coordonnées d'écran après calcul de projection.
     */
    public void setScreenCoordinates(int x, int y) {
        this.x = x;
        this.y = y;
    }
}
