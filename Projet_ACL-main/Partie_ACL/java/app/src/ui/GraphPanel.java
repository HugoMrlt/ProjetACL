package src.ui;

import src.modeles.*;
import javax.swing.*;
import java.awt.*;
import java.util.List;

public class GraphPanel extends JPanel {
    private final List<Vertex> vertices;
    private final List<Edge> edges;

    public GraphPanel(Graphe data) {
        this.vertices = data.getVertices();
        this.edges = data.getEdges();
        setBackground(Color.WHITE);
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        if (vertices.isEmpty()) return;

        Graphics2D g2 = (Graphics2D) g;
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        RenderContext ctx = calculateScale();
        drawEdges(g2, ctx);
        drawVertices(g2, ctx);
    }

    private RenderContext calculateScale() {
        double minLat = Double.MAX_VALUE, maxLat = -Double.MAX_VALUE;
        double minLon = Double.MAX_VALUE, maxLon = -Double.MAX_VALUE;

        for (Vertex v : vertices) {
            minLat = Math.min(minLat, v.getLatitude());
            maxLat = Math.max(maxLat, v.getLatitude());
            minLon = Math.min(minLon, v.getLongitude());
            maxLon = Math.max(maxLon, v.getLongitude());
        }

        // Augmente le padding à 100 pour laisser de la place aux noms des villes
        int padding = 100;
        int w = getWidth() - (padding * 2);
        int h = getHeight() - (padding * 2);

        // Sécurité contre la division par zéro si une seule ville est présente
        double rangeX = Math.max(0.0001, maxLon - minLon);
        double rangeY = Math.max(0.0001, maxLat - minLat);

        // L'échelle doit être la même pour X et Y pour ne pas déformer la carte
        double scale = Math.min(w / rangeX, h / rangeY);

        // Calcul des offsets pour bien centrer le dessin dans le panel
        int xOffset = (int) (padding + (w - rangeX * scale) / 2);
        int yOffset = (int) (padding + (h - rangeY * scale) / 2);

        // IMPORTANT : On passe maxLat pour l'inversion Y
        return new RenderContext(minLon, maxLat, scale, xOffset, yOffset);
    }

    private void drawEdges(Graphics2D g2, RenderContext ctx) {
        for (Edge edge : edges) {
            // Récupère l'enum correspondant au texte envoyé par le C++
            TypeRoute type = TypeRoute.fromString(edge.getRoadType());

            // Applique la couleur définie dans l'enum
            g2.setColor(type.getColor());

            // Applique l'épaisseur (Stroke) définie dans l'enum
            g2.setStroke(new BasicStroke(type.getStrokeWidth()));

            // Projection des coordonnées
            int x1 = ctx.projectX(edge.getVertex1().getLongitude());
            int y1 = ctx.projectY(edge.getVertex1().getLatitude());
            int x2 = ctx.projectX(edge.getVertex2().getLongitude());
            int y2 = ctx.projectY(edge.getVertex2().getLatitude());

            g2.drawLine(x1, y1, x2, y2);
        }
    }

    private void drawVertices(Graphics2D g2, RenderContext ctx) {
        for (Vertex v : vertices) {
            int x = ctx.projectX(v.getLongitude());
            int y = ctx.projectY(v.getLatitude());

            g2.setColor(Color.BLACK);
            g2.fillOval(x - 4, y - 4, 8, 8); // Points un peu plus petits

            // Un petit décalage pour ne pas que le texte touche le point
            g2.drawString(v.getName(), x + 10, y + 5);
        }
    }
}