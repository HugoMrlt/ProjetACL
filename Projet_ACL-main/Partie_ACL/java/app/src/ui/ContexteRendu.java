package src.ui;

/**
 * Contexte de rendu pour les graphiques.
 * Permet de gérer la projection des coordonnées géographiques vers les coordonnées d'écran.
 */
public class ContexteRendu {
    /**
     * Longitude minimale pour le calcul du cadrage.
     */
    private final double minLon;

    /**
     * Latitude maximale pour le calcul du cadrage
     */
    private final double maxLat;

    /**
     * Facteur d'échelle pour la conversion des coordonnées.
     */
    private final double facteurEchelle;

    /**
     * Décalage horizontal sur l'écran.
     */
    private final int decalageX;

    /**
     * Décalage vertical sur l'écran.
     */
    private final int decalageY;

    /**
     * Constructeur.
     * @param minLon - Longitude minimale.
     * @param maxLat - Latitude maximale (utilisée pour inverser l'axe Y).
     * @param facteurEchelle - Facteur d'échelle.
     * @param decalageX - Décalage en X.
     * @param decalageY - Décalage en Y.
     */
    public ContexteRendu(double minLon, double maxLat, double facteurEchelle, int decalageX, int decalageY) {
        this.minLon = minLon;
        this.maxLat = maxLat;
        this.facteurEchelle = facteurEchelle;
        this.decalageX = decalageX;
        this.decalageY = decalageY;
    }

    /**
     * Projette une longitude vers une coordonnée X sur l'écran.
     * @param longitude - La longitude à projeter.
     * @return - La coordonnée X correspondante.
     */
    public int projectionX(double longitude) {
        return (int) ((longitude - minLon) * facteurEchelle) + decalageX;
    }

    /**
     * Projette une latitude vers une coordonnée Y sur l'écran.
     * @param latitude - La latitude à projeter.
     * @return - La coordonnée Y correspondante.
     */
    public int projectionY(double latitude) {
        // On part du haut (yOffset) et on descend proportionnellement à la distance depuis la latitude la plus haute.
        return (int) ((maxLat - latitude) * facteurEchelle) + decalageY;
    }
}