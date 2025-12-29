package src.modeles;

import java.awt.Color;

public enum TypeRoute {

    // 1. Les instances (doivent correspondre au constructeur ci-dessous)
    AUTOROUTE("Autoroute", Color.BLUE, 4, 90.0),
    VOIE_RAPIDE("Voie rapide", Color.RED, 3, 80.0),
    ROUTE_EUROPEENNE("Route européenne", new Color(0, 128, 0), 3, 80.0),
    NATIONALE("Nationale", Color.ORANGE, 2, 70.0),
    DEPARTEMENTALE("Départementale", new Color(200, 200, 0), 2, 60.0),
    COMMUNALE("Communale", Color.GRAY, 1, 40.0),
    INCONNU("Inconnu", Color.LIGHT_GRAY, 1, 0.0);

    // 2. Les variables d'instance
    private final String label;
    private final Color color;
    private final int strokeWidth;
    private final double vitesse;

    // 3. LE CONSTRUCTEUR (C'est lui qui posait problème)
    // Il doit être "package-private" ou "private"
    TypeRoute(String label, Color color, int strokeWidth, double vitesse) {
        this.label = label;
        this.color = color;
        this.strokeWidth = strokeWidth;
        this.vitesse = vitesse;
    }

    // 4. Les getters (pour que ClientThread puisse les lire)
    public Color getColor() { return color; }
    public int getStrokeWidth() { return strokeWidth; }
    public String getLabel() { return label; }
    public double getVitesse() { return vitesse; }

    public static TypeRoute fromString(String text) {
        for (TypeRoute b : TypeRoute.values()) {
            if (b.label.equalsIgnoreCase(text)) {
                return b;
            }
        }
        return INCONNU;
    }
}