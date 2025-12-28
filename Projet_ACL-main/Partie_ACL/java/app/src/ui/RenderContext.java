package src.ui;

public class RenderContext {
    private final double minLon, maxLat; // On utilise maxLat pour inverser l'axe Y
    private final double scale;
    private final int xOffset, yOffset;

    public RenderContext(double minLon, double maxLat, double scale, int xOffset, int yOffset) {
        this.minLon = minLon;
        this.maxLat = maxLat;
        this.scale = scale;
        this.xOffset = xOffset;
        this.yOffset = yOffset;
    }

    public int projectX(double longitude) {
        return (int) ((longitude - minLon) * scale) + xOffset;
    }

    public int projectY(double latitude) {
        // On part du haut (yOffset) et on descend proportionnellement
        // à la distance depuis la latitude la plus haute (le Nord)
        return (int) ((maxLat - latitude) * scale) + yOffset;
    }
}