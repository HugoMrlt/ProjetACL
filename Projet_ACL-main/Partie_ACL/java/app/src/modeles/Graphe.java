package src.modeles;

import java.util.List;

public class Graphe {
    private final String windowName;
    private final List<Vertex> vertices;
    private final List<Edge> edges;

    public Graphe(String windowName, List<Vertex> vertices, List<Edge> edges) {
        this.windowName = windowName;
        this.vertices = vertices;
        this.edges = edges;
    }

    // Getters
    public String getWindowName() { return windowName; }
    public List<Vertex> getVertices() { return vertices; }
    public List<Edge> getEdges() { return edges; }
}