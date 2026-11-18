# Technische Analyse – lokale Fehlerkorrektur in B.4.9

## Methodische Basis

B.4.9 verwendet unverändert das neuronale B.4.8-System mit 32 Neuronen,
lokalem G3, `active_phase`-Eligibility, ternärer Drei-Faktor-Plastizität,
geschützter Exploration und lernbarer lokaler Hemmung. Wachstum und globale
Handlungswahl bleiben deaktiviert. Der B.4.8-Runner wird als unveränderte
Referenz eingebettet; der einzige methodische Unterschied der zentralen
B.4.9-Auswertung ist die vorgeschriebene Trennung von E1 und E2.

Nach jedem vollständigen Training entstehen zwei direkte Runtime-Kopien. Ihre
Fingerabdrücke müssen vor der ersten Auswertung mit dem trainierten Zustand
übereinstimmen. E1 verarbeitet ausschließlich den Reiz. E2 verarbeitet
dieselbe äußere Reizfolge in einer eigenen Kopie und aktiviert die geschützte
Exploration nur, wenn keine autonome Ausgabe entsteht. In beiden Kopien ist
Plastizität während der Auswertung ausgeschlossen.

384/384 Referenzläufe über drei Stromfamilien und beide Zielzuordnungen
reproduzieren die gespeicherten E2-Verwechslungsmatrizen und alle sechs
trainierten Gewichte exakt. Die nun getrennte E1-Auswertung darf von den alten
Werten abweichen: In B.4.8 beeinflusste die E2-Exploration eines Versuchs den
temporalen Zustand vor dem folgenden E1-Versuch. Für Familie 0 normal ändert
sich der E1-Mittelwert dadurch von 43,82 % auf 43,68 %, invers von 50,14 % auf
49,61 %. Das ist eine Auswertungskorrektur, keine Lernänderung.

## Auswahl einzelner Fehlentscheidungen

Für jeden Seed und Reiz wird während des unveränderten K0-Trainings die erste
eindeutige autonome Fehlantwort gesucht. Die vollständige Runtime unmittelbar
vor dieser Episode wird kopiert. Dadurch stammt die Diagnose aus einem realen
Trainingszustand und nicht aus einem nachträglich konstruierten Endzustand.
122 solche Fälle waren verfügbar.

Die Ereignisprotokolle enthalten Feuer- und Impulsereignisse, simulierte Zeit,
Synapse, Gewicht, Wirkungsart, Membran vor und nach Integration sowie die Zahl
lokaler Eligibility-Kandidaten. Negative Modulationsänderungen und die
anschließende Reizwiederholung werden in derselben Kette ausgewiesen.

## Lokale Wirkung negativer Modulation

Die negative Bewertung erreicht aktive lokale Spuren. Im Mittel ändern sich
1,754 Reizsynapsen und 0,230 Hemmsynapsen pro Fehlentscheidung. Das Gewicht
des zuvor gewählten Reizpfads sinkt von 1,484 auf 0,484; der konkurrierende
Pfad sinkt wegen gemeinsam aktiver lokaler Kandidaten ebenfalls von 1,836 auf
1,230. Nach sofortiger Bewertung wird die identische falsche eindeutige
Antwort in keinem der 122 Fälle wiederholt. 74 Fälle wechseln zur richtigen
eindeutigen Antwort, 45 bleiben ohne Ausgabe und drei aktivieren beide Wege.

Wird die Modulation 16 oder 32 Zeiteinheiten verzögert, sind die relevanten
Spuren abgelaufen. Dann bleiben 33 eindeutige Fehlantworten bestehen. Bei
Verzögerungen von 0, 4 oder 8 entstehen dieselben 74 richtigen, 45 fehlenden
und drei doppelten Antworten wie bei unmittelbarer Modulation. Damit ist die
lokale Eligibility-Zeit kausal; ein nachwirkender globaler Korrekturzustand
existiert nicht.

## Erregende und inhibitorische Konkurrenz

Alle Eingriffe erfolgen an Kopien desselben Nach-Feedback-Zustands:

- Kontrolle, deaktivierte inhibitorische Wirkung und zurückgesetzte
  Hemmgewichte liefern exakt dieselbe Verteilung von 41 A-, 33 B-, 45
  fehlenden und drei doppelten Antworten.
- Die Reduktion ausschließlich der zuvor dominierenden Reizsynapse beseitigt
  die drei Doppelantworten, erzeugt aber nur drei zusätzliche eindeutige
  Ausgaben und behebt die 45 Ausfälle nicht.
- Ein isolierter allgemeiner Impuls erreicht den jeweils unterlegenen
  motorischen Pfad in 122/122 Kopien.
- Wiederholungsabstände von 0, 4, 8, 16 und 32 Zeiteinheiten verändern das
  Ergebnis nicht. Die technisch gleichzeitige Integration bleibt durch die
  bestehenden B.4.6-Reihenfolgetests abgesichert.

Die Alternative ist somit strukturell vorhanden. Weder gespeicherte Hemmung
noch eine refraktäre oder zeitliche Nachwirkung erklärt den Hauptausfall.

## Bindung einer erfolgreichen Alternative

Nach negativer Bewertung wird derselbe Reiz erneut neuronal präsentiert und
danach ein allgemeiner Explorationspfad aktiviert. Der Runner kennt die
Zielhandlung nur für die äußere Bewertung. Er teilt dem Netz weder mit, dass
die Handlung eine Alternative ist, noch welcher Motorpfad richtig wäre.

67 Explorationsereignisse aktivieren den zuvor nicht gewählten Pfad und sind
als gesamte neuronale Antwort positiv bewertbar. Vor der Rückmeldung bestehen
im Mittel 3,25 aktive Eligibility-Entscheidungen; die positive Modulation
ändert im Mittel 1,06 Reiz- und 0,36 Hemmsynapsen. Trotzdem wird die richtige
Antwort beim nächsten gleichen Reiz nur in 35/67 Fällen autonom abgerufen.
Zwölf Fälle bleiben still, 20 fallen auf die falsche eindeutige Antwort
zurück. Die zweite Reizbeziehung bleibt in 45/67 Fällen richtig; beide
Beziehungen zugleich bleiben nur in 21/67 Fällen erhalten.

Mehrfache negative Bewertung stabilisiert ebenfalls keinen Ersatzpfad. Nach
der vierten Wiederholung bleiben 81/122 Zustände ohne Antwort. Negative
Erfahrung schwächt daher wirksam die aktuelle Entscheidung, aber wiederholte
Abschwächung erodiert Aktivität schneller, als positive Alternativerfahrung
eine selektive Reizbindung stabilisiert.

## Entscheidung gegen K7

Die Diagnose identifiziert keinen einzelnen fehlenden lokalen
Aktivierungszustand: Die unmittelbar falsche Antwort wird bereits durch die
bestehende Plastizität beseitigt, Hemmungs- und Timingablationen ändern den
Ausfall nicht, und der alternative Pfad ist erreichbar. Der verbleibende
Engpass umfasst die selektive Zuordnung und Stabilisierung positiver
Alternativerfahrung bei gleichzeitigem Erhalt der zweiten Beziehung.

Ein neues Erregbarkeits-, Hemmungs- oder Bereitschaftsbit würde diesen Befund
nicht isoliert beheben und wäre ohne weiteren Kausalnachweis eine zusätzliche
Hypothese. K7 und folglich K8 werden deshalb nicht implementiert. Es entsteht
kein neuer persistenter neuronaler Zustand.

## Ressourcen und Regression

Alle ausgewerteten Zustände sind gültig, bleiben bei etwa 24,5 KiB und einer
maximalen Queue von 26 Ereignissen begrenzt. 64/64 Persistenzproben werden mit
identischem Fingerabdruck restauriert und deterministisch fortgesetzt.
Die bisherigen 459 Tests sowie 14 neue B.4.9-Prüfungen bestehen: 473/473.

