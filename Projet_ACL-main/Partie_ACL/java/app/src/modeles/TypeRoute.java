package src.modeles;

import java.awt.Color;

public enum TypeRoute {
    /**
     * Les instances (doivent correspondre au constructeur ci-dessous).
     */
    AUTOROUTE("Autoroute", Color.BLUE, 4, 90.0),
    VOIE_RAPIDE("Voie rapide", Color.RED, 3, 80.0),
    ROUTE_EUROPEENNE("Route européenne", new Color(0, 128, 0), 3, 80.0),
    NATIONALE("Nationale", Color.ORANGE, 2, 70.0),
    DEPARTEMENTALE("Départementale", new Color(200, 200, 0), 2, 60.0),
    COMMUNALE("Communale", Color.DARK_GRAY, 1, 40.0),
    AUCUN_TYPE("Aucun Type", Color.MAGENTA, 1, 0.0),
    INCONNU("Inconnu", Color.MAGENTA, 1, 0.0);

    /**
     * Libellé du type de route.
     */
    private final String label;

    /**
     * Couleur du type de route.
     */
    private final Color couleur;

    /**
     * Epaisseur de la ligne du type de route.
     */
    private final int epaisseur;

    /**
     * Vitesse d'un camion sur cette route.
     */
    private final double vitesse;

    /**
     * Constructeur.
     * @param label - Libellé du type de route.
     * @param color - Couleur du type de route.
     * @param epaisseur - Epaisseur de la ligne du type de route.
     * @param vitesse - Vitesse d'un camion sur cette route.
     */
    TypeRoute(String label, Color color, int epaisseur, double vitesse) {
        this.label = label;
        this.couleur = color;
        this.epaisseur = epaisseur;
        this.vitesse = vitesse;
    }

    /**
     * Retourne la couleur du type de route.
     * @return - Couleur du type de route.
     */
    public Color getColor() { return couleur; }

    /**
     * Retourne l'épaisseur de la ligne du type de route.
     * @return - Epaisseur de la ligne du type de route.
     */
    public int getEpaisseur() { return epaisseur; }

    /**
     * Retourne le libellé du type de route.
     * @return - Libellé du type de route.
     */
    public String getLabel() { return label; }

    /**
     * Retourne la vitesse d'un camion sur cette route.
     * @return - Vitesse d'un camion sur cette route.
     */
    public double getVitesse() { return vitesse; }

    /**
     * Constructeur.
     * Convertie un texte en TypeRoute.
     *
     * @param text - Texte représentant un TypeRoute.
     * @return - Le type de route correspondant, ou INCONNU si aucun type ne correspond au texte.
     */
    public static TypeRoute depuisTexte(String text) {
        for (TypeRoute b : TypeRoute.values()) {
            if (b.label.equalsIgnoreCase(text)) {
                return b;
            }
        }
        return INCONNU;
    }
}