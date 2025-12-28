package src;

import src.modeles.Edge;
import src.modeles.Graphe;
import src.modeles.Vertex;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import org.json.JSONArray;
import org.json.JSONObject;

/**
 * Classe utilitaire pour traiter les données des sommets et des arêtes
 * avec AWT et gestion des types de routes.
 */
public class Utils {

    public static Graphe parseJson(BufferedReader reader) throws IOException {
        StringBuilder sb = new StringBuilder();
        String line;
        while ((line = reader.readLine()) != null) {
            sb.append(line);
        }

        String json = sb.toString();
        if (json.isEmpty()) return null;

        String name = readWindowName(json);
        List<Vertex> v = readVertices(json);
        List<Edge> e = readEdges(json, v);

        return new Graphe(name, v, e);
    }
    
    private static void convertCoordinatesToScreen(List<Vertex> vertices) {
        // Supposons que LogistiqueApp possède toujours ces dimensions
        double width = 800; // Ou LogistiqueApp.width;
        double height = 600; // Ou LogistiqueApp.height;

        double padding = 50;
        double drawableWidth = width - 2 * padding;
        double drawableHeight = height - 2 * padding;

        double minLat = Double.MAX_VALUE, maxLat = Double.MIN_VALUE;
        double minLon = Double.MAX_VALUE, maxLon = Double.MIN_VALUE;

        for (Vertex vertex : vertices) {
            if (vertex.getLongitude() < minLon) minLon = vertex.getLongitude();
            if (vertex.getLongitude() > maxLon) maxLon = vertex.getLongitude();
            if (vertex.getLatitude() < minLat) minLat = vertex.getLatitude();
            if (vertex.getLatitude() > maxLat) maxLat = vertex.getLatitude();
        }

        double rangeX = maxLon - minLon;
        double rangeY = maxLat - minLat;

        double scale = Math.min(drawableWidth / rangeX, drawableHeight / rangeY);

        double offsetX = (width - (rangeX * scale)) / 2;
        double offsetY = (height - (rangeY * scale)) / 2;

        for (Vertex vertex : vertices) {
            // Calcul du passage monde-écran imposé [cite: 118]
            int x = (int) (offsetX + (vertex.getLongitude() - minLon) * scale);
            int y = (int) (offsetY + (vertex.getLatitude() - minLat) * scale);

            // Inversion Y car l'origine (0,0) AWT est en haut à gauche
            y = (int) height - y;

            vertex.setScreenCoordinates(x, y);
        }
    }

    public static String readWindowName(String data) {
        JSONObject jsonData = new JSONObject(data);
        return jsonData.getString("windowName");
    }

    public static List<Vertex> readVertices(String data) {
        JSONObject jsonData = new JSONObject(data);
        JSONArray jsonVertices = jsonData.getJSONArray("vertices");
        List<Vertex> vertices = new ArrayList<>();

        for (int i = 0; i < jsonVertices.length(); i++) {
            JSONObject jsonVertex = jsonVertices.getJSONObject(i);
            // On utilise latitude/longitude comme dans les fichiers JSON du sujet [cite: 11]
            String name = jsonVertex.getString("ville");
            double lat = jsonVertex.getDouble("latitude");
            double lon = jsonVertex.getDouble("longitude");
            vertices.add(new Vertex(name, lat, lon));
        }

        return vertices;
    }

    /**
     * Extrait les arêtes incluant désormais le TYPE DE ROUTE.
     */
    public static List<Edge> readEdges(String data, List<Vertex> vertices) {
        JSONObject jsonData = new JSONObject(data);
        JSONArray jsonEdges = jsonData.getJSONArray("edges");
        List<Edge> edges = new ArrayList<>();

        for (int i = 0; i < jsonEdges.length(); i++) {
            JSONObject jsonEdge = jsonEdges.getJSONObject(i);
            String v1Name = jsonEdge.getString("v1");
            String v2Name = jsonEdge.getString("v2");
            int weight = jsonEdge.getInt("poids");
            String roadType = jsonEdge.optString("type", "communale");

            Vertex v1 = null, v2 = null;
            for (Vertex v : vertices) {
                if (v.getName().equals(v1Name)) v1 = v;
                if (v.getName().equals(v2Name)) v2 = v;
            }

            if (v1 != null && v2 != null) {
                // Création de l'arête avec le type de route pour la visualisation [cite: 17, 119]
                edges.add(new Edge(v1, v2, weight, roadType));
            }
        }
        return edges;
    }
}