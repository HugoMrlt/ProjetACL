package src.ui;

import src.UtilitaireCalcul;
import src.modeles.Graphe;
import src.ThreadClient;
import src.modeles.Sommet;
import src.modeles.Arete;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import javax.swing.border.LineBorder;
import javax.swing.border.CompoundBorder;
import javax.swing.border.MatteBorder;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

/**
 * Panneau latéral de configuration et de contrôle.
 * Permet à l'utilisateur de charger une carte, de paramétrer la mission et de filtrer les résultats.
 */
public class PanneauControle extends JPanel {

    private final JComboBox<String> regionComboBox, modeComboBox, departComboBox, selecteurCamion;
    private final JSpinner camionsSpinner;
    private final JButton boutonCharger, boutonLancer, boutonVoirDernier;
    private final PanneauGraphe panneauGraphe;
    private Graphe dernierResultat = null;
    private final JPanel panneauContenu;
    private final JButton boutonReduire;
    private final JLabel coutDistanceValeur;
    private final JLabel coutDureeValeur;

    // Constantes de style pour l'interface.
    private final Color COULEUR_FOND = Color.WHITE;
    private final Color ENTETE_FOND = new Color(30, 30, 30);
    private final Color ENTETE_TEXTE = Color.WHITE;
    private final Color BOUTON_FOND = Color.WHITE;
    private final Color BOUTON_TEXTE = Color.BLACK;
    private final Color BOUTON_DESACTIVE_FOND = new Color(220, 220, 220);
    private final Color BOUTON_DESACTIVE_TEXTE = new Color(120, 120, 120);
    private final Font POLICE_UI = new Font("Segoe UI", Font.PLAIN, 13);
    private final Font POLICE_GRAS = new Font("Segoe UI", Font.BOLD, 13);

    /**
     * Constructeur.
     *
     * @param panneauGraphe - Référence vers le panneau d'affichage pour les mises à jour.
     */
    public PanneauControle(PanneauGraphe panneauGraphe) {
        this.panneauGraphe = panneauGraphe;
        setLayout(new BorderLayout());
        setBackground(COULEUR_FOND);
        setBorder(new MatteBorder(0, 1, 0, 0, Color.BLACK));

        // Création de l'entête du panneau.
        JPanel panneauEntete = new JPanel(new BorderLayout());
        panneauEntete.setBackground(ENTETE_FOND);
        panneauEntete.setBorder(new EmptyBorder(12, 15, 12, 15));
        panneauEntete.setPreferredSize(new Dimension(320, 50));

        JLabel labelTitre = new JLabel("CONFIGURATION");
        labelTitre.setFont(new Font("Segoe UI", Font.BOLD, 14));
        labelTitre.setForeground(ENTETE_TEXTE);

        boutonReduire = new JButton("▼");
        stylerBoutonEntete(boutonReduire);
        boutonReduire.addActionListener(e -> basculerContenu());

        panneauEntete.add(labelTitre, BorderLayout.WEST);
        panneauEntete.add(boutonReduire, BorderLayout.EAST);
        add(panneauEntete, BorderLayout.NORTH);

        // Panneau principal contenant les contrôles (GridBagLayout pour l'alignement).
        panneauContenu = new JPanel(new GridBagLayout());
        panneauContenu.setBackground(COULEUR_FOND);
        panneauContenu.setBorder(new EmptyBorder(20, 20, 20, 20));
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(8, 0, 8, 0);
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.weightx = 1.0;
        gbc.gridx = 0;

        // SECTION 1 : CHARGEMENT DE LA CARTE.
        ajouterTitreSection(panneauContenu, "1. CHOIX DE LA CARTE", gbc);
        regionComboBox = new JComboBox<>(UtilitaireRegions.getNomsRegions().toArray(new String[0]));
        styleCombo(regionComboBox);
        panneauContenu.add(regionComboBox, gbcSuivant(gbc));

        boutonCharger = new JButton("CHARGER LA CARTE");
        stylerBoutonAction(boutonCharger);
        panneauContenu.add(boutonCharger, gbcSuivant(gbc));

        boutonVoirDernier = new JButton("Revoir le dernier résultat");
        stylerBoutonLien(boutonVoirDernier);
        boutonVoirDernier.setEnabled(false);
        panneauContenu.add(boutonVoirDernier, gbcSuivant(gbc));

        ajouterSeparateur(panneauContenu, gbc);

        // SECTION 2 : PARAMÈTRES DE LA MISSION.
        ajouterTitreSection(panneauContenu, "2. PARAMÈTRES MISSION", gbc);
        panneauContenu.add(creerLabel("Mode de calcul :"), gbcSuivant(gbc));
        modeComboBox = new JComboBox<>(new String[]{"DISTANCE", "TEMPS"});
        styleCombo(modeComboBox);
        panneauContenu.add(modeComboBox, gbcSuivant(gbc));

        panneauContenu.add(creerLabel("Ville de départ :"), gbcSuivant(gbc));
        departComboBox = new JComboBox<>();
        styleCombo(departComboBox);
        departComboBox.setEnabled(false);
        panneauContenu.add(departComboBox, gbcSuivant(gbc));

        panneauContenu.add(creerLabel("Nombre de camions :"), gbcSuivant(gbc));
        camionsSpinner = new JSpinner(new SpinnerNumberModel(1, 1, Integer.MAX_VALUE, 1));
        panneauContenu.add(camionsSpinner, gbcSuivant(gbc));

        ajouterSeparateur(panneauContenu, gbc);

        // SECTION 3 : RÉSULTATS.
        ajouterTitreSection(panneauContenu, "3. RÉSULTATS PAR CAMION", gbc);
        selecteurCamion = new JComboBox<>(new String[]{"Tous les camions"});
        styleCombo(selecteurCamion);
        selecteurCamion.addActionListener(e -> {
            String sel = (String) selecteurCamion.getSelectedItem();
            if (sel == null || sel.equals("Tous les camions")) {
                panneauGraphe.setFiltreCamion(-1);
                mettreAJourCout();
            } else {
                int id = Integer.parseInt(sel.replace("Camion ", ""));
                panneauGraphe.setFiltreCamion(id);
                mettreAJourCout();
            }
        });
        panneauContenu.add(selecteurCamion, gbcSuivant(gbc));

        // Bloc coût total (distance & durée estimée).
        JPanel blocCout = new JPanel(new GridLayout(2, 2, 6, 4));
        blocCout.setBackground(COULEUR_FOND);
        blocCout.add(creerLabel("Distance totale :"));
        coutDistanceValeur = creerLabel("N/A");
        blocCout.add(coutDistanceValeur);
        blocCout.add(creerLabel("Durée estimée :"));
        coutDureeValeur = creerLabel("N/A");
        blocCout.add(coutDureeValeur);
        panneauContenu.add(blocCout, gbcSuivant(gbc));

        ajouterSeparateur(panneauContenu, gbc);

        boutonLancer = new JButton("LANCER LE CALCUL");
        stylerBoutonAction(boutonLancer);
        boutonLancer.setEnabled(false);
        gbc.insets = new Insets(25, 0, 0, 0);
        panneauContenu.add(boutonLancer, gbcSuivant(gbc));

        gbc.weighty = 1.0;
        panneauContenu.add(Box.createVerticalGlue(), gbcSuivant(gbc));
        add(new JScrollPane(panneauContenu), BorderLayout.CENTER);

        initialiserLogique();
    }

    /**
     * Met à jour la liste déroulante des camions après un calcul.
     *
     * @param graphe - Le graphe.
     */
    private void updateTruckList(Graphe graphe) {
        selecteurCamion.removeAllItems();
        selecteurCamion.addItem("Tous les camions");
        int max = 1;

        for (Arete e : graphe.getAretes())
            if (e.getCamionId() > max)
                max = e.getCamionId();

        for (int i = 1; i <= max; i++)
            selecteurCamion.addItem("Camion " + i);

        selecteurCamion.setSelectedIndex(0);
        mettreAJourCout();
    }

    /**
     * Initialise les écouteurs d'événements pour les boutons.
     */
    private void initialiserLogique() {
        boutonCharger.addActionListener(e -> {
            String region = (String) regionComboBox.getSelectedItem();
            if (region != null) chargerDonneesRegion(region, UtilitaireRegions.getCheminFichierRegion(region));
        });

        boutonVoirDernier.addActionListener(e -> {
            if (dernierResultat != null) {
                panneauGraphe.setGraphe(dernierResultat);
                mettreAJourCout();
            }
        });

        boutonLancer.addActionListener(e -> {
            String commande = regionComboBox.getSelectedItem() + ";" + modeComboBox.getSelectedItem() + ";" + departComboBox.getSelectedItem() + ";" + camionsSpinner.getValue();

            new Thread(() -> {
                new ThreadClient(commande) {
                    public void traiterResultat(Graphe graphe, String label) {
                        if (graphe != null) SwingUtilities.invokeLater(() -> {
                            dernierResultat = graphe;
                            boutonVoirDernier.setEnabled(true);
                            panneauGraphe.setGraphe(graphe);
                            updateTruckList(graphe);
                        });
                    }
                }.start();
            }).start();
        });
    }

    /**
     * Charge les données d'une région depuis un fichier JSON.
     *
     * @param nomRegion - Nom de la région.
     * @param fichier   - Chemin du fichier JSON.
     */
    private void chargerDonneesRegion(String nomRegion, String fichier) {
        try (java.io.BufferedReader reader = new java.io.BufferedReader(new java.io.FileReader(fichier))) {
            StringBuilder sb = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) sb.append(line);
            String content = sb.toString().trim();
            Graphe graphe = null;

            if (content.startsWith("[")) {
                // Format simple : liste de villes.
                org.json.JSONArray array = new org.json.JSONArray(content);
                List<Sommet> vertices = new ArrayList<>();

                for (int i = 0; i < array.length(); i++) {
                    org.json.JSONObject obj = array.getJSONObject(i);
                    vertices.add(new Sommet(obj.getString("ville"), obj.getDouble("latitude"), obj.getDouble("longitude")));
                }

                graphe = new Graphe(nomRegion, vertices, new ArrayList<>());
            } else {
                // Format complexe : graphe complet via UtilitaireCalcul.
                try (java.io.BufferedReader br = new java.io.BufferedReader(new java.io.StringReader(content))) {
                    graphe = UtilitaireCalcul.parseJson(br);
                }
            }
            if (graphe != null) {
                panneauGraphe.setGraphe(graphe);
                departComboBox.removeAllItems();

                for (Sommet v : graphe.getSommets())
                    departComboBox.addItem(v.getNom());

                departComboBox.setEnabled(true);
                boutonLancer.setEnabled(true);
                updateTruckList(graphe);
            } else
                reinitialiserInterfaceSurErreur();
        } catch (Exception ex) {
            reinitialiserInterfaceSurErreur();
        }
    }

    /**
     * Réinitialise l'interface en cas d'erreur.
     */
    private void reinitialiserInterfaceSurErreur() {
        departComboBox.removeAllItems();
        departComboBox.setEnabled(false);
        boutonLancer.setEnabled(false);
        coutDistanceValeur.setText("N/A");
        coutDureeValeur.setText("N/A");
    }

    // MÉTHODES DE STYLE ET UTILITAIRES DE MISE EN PAGE.

    /**
     * Définit l'apparence visuelle des boutons d'action.
     *
     * @param btn - Le bouton à styliser.
     */
    private void stylerBoutonAction(JButton btn) {
        btn.setFont(POLICE_GRAS);
        btn.setFocusPainted(false);
        btn.setCursor(new Cursor(Cursor.HAND_CURSOR));
        btn.addPropertyChangeListener("enabled", evt -> mettreAJourStyleBouton(btn));
        mettreAJourStyleBouton(btn);
    }

    /**
     * Met à jour l'apparence du bouton selon son état (activé ou désactivé).
     *
     * @param btn - Le bouton à mettre à jour.
     */
    private void mettreAJourStyleBouton(JButton btn) {
        if (btn.isEnabled()) {
            btn.setBackground(BOUTON_FOND);
            btn.setForeground(BOUTON_TEXTE);
            btn.setBorder(new CompoundBorder(new LineBorder(Color.BLACK, 2), new EmptyBorder(10, 15, 10, 15)));
        } else {
            btn.setBackground(BOUTON_DESACTIVE_FOND);
            btn.setForeground(BOUTON_DESACTIVE_TEXTE);
            btn.setBorder(new CompoundBorder(new LineBorder(Color.GRAY, 2), new EmptyBorder(10, 15, 10, 15)));
        }
    }

    /**
     * Définit l'apparence visuelle du bouton de l'entête.
     *
     * @param b - Le bouton à styliser.
     */
    private void stylerBoutonEntete(JButton b) {
        b.setFocusPainted(false);
        b.setContentAreaFilled(false);
        b.setBorderPainted(false);
        b.setFont(new Font("Segoe UI", Font.BOLD, 16));
        b.setForeground(Color.WHITE);
        b.setCursor(new Cursor(Cursor.HAND_CURSOR));
    }

    /**
     * Définit l'apparence visuelle des boutons de type lien.
     *
     * @param b - Le bouton à styliser.
     */
    private void stylerBoutonLien(JButton b) {
        b.setFocusPainted(false);
        b.setContentAreaFilled(false);
        b.setBorderPainted(false);
        b.setFont(new Font("Segoe UI", Font.ITALIC, 12));
        b.setForeground(Color.BLUE.darker());
        b.setCursor(new Cursor(Cursor.HAND_CURSOR));
    }

    /**
     * Définit l'apparence visuelle des listes déroulantes (combobox).
     *
     * @param b - La liste déroulante à styliser.
     */
    private void styleCombo(JComboBox b) {
        b.setBackground(Color.WHITE);
        b.setFont(POLICE_UI);
    }

    /**
     * Crée un libellé (JLabel) avec la police standard du panneau.
     *
     * @param t - Le texte du libellé.
     * @return - Le composant JLabel créé.
     */
    private JLabel creerLabel(String t) {
        JLabel l = new JLabel(t);
        l.setFont(POLICE_UI);
        l.setForeground(Color.BLACK);

        return l;
    }

    /**
     * Ajoute un titre de section stylisé au panneau.
     *
     * @param p - Le panneau parent.
     * @param t - Le texte du titre.
     * @param gbc - Les contraintes de disposition.
     */
    private void ajouterTitreSection(JPanel p, String t, GridBagConstraints gbc) {
        JLabel l = new JLabel(t);
        l.setFont(new Font("Segoe UI", Font.BOLD, 11));
        l.setForeground(Color.BLACK);
        l.setBorder(new MatteBorder(0, 0, 2, 0, Color.BLACK));
        gbc.gridy++;
        gbc.insets = new Insets(15, 0, 10, 0);
        p.add(l, gbc);
        gbc.insets = new Insets(5, 0, 5, 0);
    }

    /**
     * Ajoute un séparateur au panneau.
     * @param p - Le panneau.
     * @param gbc - Grille pour l'alignement.
     */
    private void ajouterSeparateur(JPanel p, GridBagConstraints gbc) {
        JSeparator s = new JSeparator();
        s.setForeground(Color.LIGHT_GRAY);
        gbc.gridy++;
        gbc.insets = new Insets(15, 0, 15, 0);
        p.add(s, gbc);
        gbc.insets = new Insets(5, 0, 5, 0);
    }

    /**
     * Conteneur suivant.
     * @param g - Grille pour l'alignement.
     * @return - La nouvelle grille.
     */
    private GridBagConstraints gbcSuivant(GridBagConstraints g) {
        g.gridy++;

        return g;
    }

    /**
     * Basculer le contenu du panneau.
     */
    private void basculerContenu() {
        boolean v = panneauContenu.isVisible();
        panneauContenu.setVisible(!v);
        boutonReduire.setText(!v ? "▼" : "◀");
        revalidate();
        repaint();
    }

    /**
     * Met à jour l'affichage du coût total selon le camion sélectionné ou tout le graphe.
     */
    /**
     * Met à jour l'affichage du coût total.
     * Note : Le calcul par camion individuel doit être géré par le serveur C++
     * pour respecter la consigne de non-modélisation côté Java.
     */
    private void mettreAJourCout() {
        Graphe g = panneauGraphe.getGraphe();
        if (g == null) {
            coutDistanceValeur.setText("N/A");
            coutDureeValeur.setText("N/A");
            return;
        }

        // On récupère les valeurs globales stockées dans l'objet Graphe
        // (Valeurs qui ont été calculées par le C++ et transmises via JSON)
        double distance = g.getDistanceTotale();
        double duree = g.getDureeTotale();

        coutDistanceValeur.setText(formatDistance(distance));

        // Gestion de l'erreur de durée (si le serveur a renvoyé une valeur négative)
        if (duree < 0) {
            coutDureeValeur.setText("<html><font color='red'>ERREUR</font></html>");

            // On récupère les arêtes du graphe pour identifier celles qui n'ont pas de type
            StringBuilder msg = new StringBuilder("Certaines routes ont un type inconnu.\n");
            msg.append("Vérifiez le fichier CSV des types de routes côté Serveur.\n\n");
            msg.append("Détails des arêtes reçues :\n");

            for (Arete a : g.getAretes()) {
                if (a.getTypeRoute() == null || a.getTypeRoute().equals("Inconnu")) {
                    msg.append("• ").append(a.getSommet1().getNom())
                            .append(" ↔ ").append(a.getSommet2().getNom())
                            .append(" (type: \"").append(a.getTypeRoute()).append("\")\n");
                }
            }

            JOptionPane.showMessageDialog(this, msg.toString(), "Problème de données", JOptionPane.WARNING_MESSAGE);
        } else {
            coutDureeValeur.setText(formatDuree(duree));
        }
    }

    private String formatDistance(double km) {
        if (Double.isNaN(km)) return "N/A";
        return String.format("%.1f km", km);
    }

    private String formatDuree(double heures) {
        if (Double.isNaN(heures)) return "N/A";
        int minutes = (int) Math.round(heures * 60);
        int h = minutes / 60;
        int m = minutes % 60;
        if (h == 0) return m + " min";
        return h + " h " + m + " min";
    }
}

