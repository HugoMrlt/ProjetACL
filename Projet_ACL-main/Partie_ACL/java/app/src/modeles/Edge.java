package src.modeles;

import java.awt.Color;

public class Edge {
    private final Vertex vertex1;
    private final Vertex vertex2;
    private final int weight;
    private final String roadType; // "voie rapide", "communale", etc. [cite: 6]

    public Edge(Vertex vertex1, Vertex vertex2, int weight, String roadType) {
        this.vertex1 = vertex1;
        this.vertex2 = vertex2;
        this.weight = weight;
        this.roadType = roadType;
    }

    public Vertex getVertex1() { return vertex1; }
    public Vertex getVertex2() { return vertex2; }
    public int getWeight() { return weight; }
    public String getRoadType() { return roadType; }

    // Logique métier pour AWT : associer une couleur au type
    public Color getAwtColor() {
        return TypeRoute.fromString(this.roadType).getColor();
    }
}