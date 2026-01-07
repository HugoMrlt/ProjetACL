package src.ui;

import src.modeles.*;
import javax.swing.*;
import java.awt.*;
import java.util.List;

/**
 * Panneau d'affichage graphique du graphe.
 * Gère le dessin des sommets (villes), des arêtes (routes) et de la légende.
 */
public class PanneauGraphe extends JPanel {
    /**
     * Graphe courant (référence complète).
     */
    private Graphe graphe;

    /**
     * Liste des sommets à afficher.
     */
    private List<Sommet> sommets;

    /**
     * Liste des arêtes à afficher.
     */
    private List<Arete> aretes;

    /**
     * Identifiant du camion pour le filtrage (-1 par défaut pour afficher tous les camions).
     */
    private int filtreCamion = -1;

    /**
     * Constructeur.
     * @param graphe - Le graphe contenant les sommets et les arêtes.
     */
    public PanneauGraphe(Graphe graphe) {
        this.graphe = graphe;
        this.sommets = graphe.getSommets();
        this.aretes = graphe.getAretes();
        setBackground(Color.WHITE);
    }

    /**
     * Met à jour les données du graphe et redessine le panneau.
     * @param graphe - Les nouvelles données du graphe.
     */
    public void setGraphe(Graphe graphe) {
        this.graphe = graphe;
        this.sommets = graphe.getSommets();
        this.aretes = graphe.getAretes();
        repaint();
    }

    /**
     * Retourne le graphe actuellement affiché.
     */
    public Graphe getGraphe() {
        return graphe;
    }

    /**
     * Définit le filtre pour n'afficher que le trajet d'un camion spécifique.
     * @param id - L'identifiant du camion.
     */
    public void setFiltreCamion(int id) {
        this.filtreCamion = id;
        repaint();
    }

    /**
     * Dessine le panneau.
     * @param g - Le contexte de dessin.
     */
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        if (sommets.isEmpty()) return;

        Graphics2D g2 = (Graphics2D) g;

        // Activation de l'anti-aliasing pour un rendu plus lisse.
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        // Calcul du cadrage et de l'échelle selon la taille actuelle de la fenêtre.
        ContexteRendu ctx = calculerEchelle();

        dessinerAretes(g2, ctx);
        dessinerSommets(g2, ctx);
        dessinerLegende(g2);
    }

    /**
     * Dessine les routes (arêtes) sur la carte.
     * @param g2 - Le contexte de dessin.
     * @param ctx - Le contexte de projection.
     */
    private void dessinerAretes(Graphics2D g2, ContexteRendu ctx) {
        for (Arete arete : aretes) {
            // Application du filtre par camion.
            if (filtreCamion != -1 && arete.getCamionId() != filtreCamion)
                continue;

            // Récupération du style selon le type de route.
            TypeRoute type = TypeRoute.depuisTexte(arete.getTypeRoute());
            g2.setColor(type.getColor());
            g2.setStroke(new BasicStroke(type.getEpaisseur()));

            // Projection des coordonnées géographiques vers l'écran.
            int x1 = ctx.projectionX(arete.getSommet1().getLongitude());
            int y1 = ctx.projectionY(arete.getSommet1().getLatitude());
            int x2 = ctx.projectionX(arete.getSommet2().getLongitude());
            int y2 = ctx.projectionY(arete.getSommet2().getLatitude());

            g2.drawLine(x1, y1, x2, y2);

            // Affichage du poids (distance) au milieu de l'arête.
            int midX = (x1 + x2) / 2;
            int midY = (y1 + y2) / 2;

            g2.setColor(Color.BLACK);
            g2.setFont(new Font("Arial", Font.PLAIN, 12));
            g2.drawString(arete.getPoids() + " km", midX + 5, midY - 5);
        }
    }

    /**
     * Dessine les villes (sommets) sur la carte.
     * @param g2 - Le contexte de dessin.
     * @param ctx - Le contexte de projection.
     */
    private void dessinerSommets(Graphics2D g2, ContexteRendu ctx) {
        for (Sommet v : sommets) {
            int x = ctx.projectionX(v.getLongitude());
            int y = ctx.projectionY(v.getLatitude());

            g2.setColor(Color.BLACK);
            g2.fillOval(x - 4, y - 4, 8, 8);
            g2.drawString(v.getNom(), x + 10, y + 5);
        }
    }

    /**
     * Dessine la légende des types de routes en bas à droite.
     * @param g2 - Le contexte de dessin.
     */
    private void dessinerLegende(Graphics2D g2) {
        int nbTypes = 0;
        for (TypeRoute type : TypeRoute.values()) {
            if (type != TypeRoute.AUCUN_TYPE && type != TypeRoute.INCONNU) nbTypes++;
        }

        int x = getWidth() - 250;
        int y = getHeight() - 30 - nbTypes * 22;

        // Fond semi-transparent pour la légende.
        g2.setColor(new Color(255,255,255,220));
        g2.fillRoundRect(x - 10, y - 20, 240, 22 * nbTypes + 30, 15, 15);

        g2.setColor(Color.BLACK);
        g2.drawString("Légende :", x, y);

        int i = 1;
        for (TypeRoute type : TypeRoute.values()) {
            if (type == TypeRoute.AUCUN_TYPE || type == TypeRoute.INCONNU) continue;

            g2.setColor(type.getColor());
            g2.setStroke(new BasicStroke(type.getEpaisseur()));
            g2.drawLine(x, y + i * 22, x + 30, y + i * 22);

            g2.setColor(Color.BLACK);
            g2.drawString(type.getLabel() + " (vitesse : " + (int)type.getVitesse() + " km/h)", x + 40, y + i * 22 + 5);
            i++; // Incrémentation.
        }
    }

    /**
     * Calcule dynamiquement l'échelle et le centrage pour que le graphe remplisse le panneau.
     */
    private ContexteRendu calculerEchelle() {
        double minLat = Double.MAX_VALUE, maxLat = -Double.MAX_VALUE;
        double minLon = Double.MAX_VALUE, maxLon = -Double.MAX_VALUE;

        // Calcul des bornes du graphe actuel.
        for (Sommet v : sommets) {
            minLat = Math.min(minLat, v.getLatitude());
            maxLat = Math.max(maxLat, v.getLatitude());
            minLon = Math.min(minLon, v.getLongitude());
            maxLon = Math.max(maxLon, v.getLongitude());
        }

        int marge = 100;
        int largeurUtile = getWidth() - (marge * 2);
        int hauteurUtile = getHeight() - (marge * 2);

        double etendueX = Math.max(0.0001, maxLon - minLon);
        double etendueY = Math.max(0.0001, maxLat - minLat);

        double echelle = Math.min(largeurUtile / etendueX, hauteurUtile / etendueY);

        int decalageX = (int) (marge + (largeurUtile - etendueX * echelle) / 2);
        int decalageY = (int) (marge + (hauteurUtile - etendueY * echelle) / 2);

        return new ContexteRendu(minLon, maxLat, echelle, decalageX, decalageY);
    }
}