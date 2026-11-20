# Forschungsbericht – Forschungsstufe B.5.6

## Ergebnis

B.5.6 erklärt den größten Teil der verbleibenden nichtrobusten Lernverläufe:
Die Beziehungen werden erworben, danach aber durch sensorische
Gewichtsveränderungen wieder unzuverlässig. Eine neue neuronale Funktion ist
noch nicht gerechtfertigt.

Die historische Bestätigung reproduziert B.5.5 exakt: S4-d16 erreicht E1 X =
0,804343, E1 Y = 0,827689 und 243 robuste Netze. In der unabhängigen frischen
Bestätigung erreicht S4-d16 E1 X = 0,786228, E1 Y = 0,827779 und 236 von 384
robusten Netzen.

## Antworten auf die Abschlussfragen

**A. Welche Arten nichtrobuster Netze existieren?** 82 frische Netze scheitern
an X, 66 an Y, keines an beiden E1-Beziehungen gleichzeitig. Alle 148 zeigen
Verlust und Reakquisition; 145 sind am Ende momentan nicht gemeinsam verfügbar,
drei sind momentan korrekt, aber über E1 nicht robust.

**B. Welcher Anteil hat nie vollständig erworben?** 0 von 148 (0 %).

**C. Welcher Anteil besitzt Verbindungen, kann sie aber nicht zuverlässig
abrufen?** Alle 148 hatten beide Beziehungen erworben. Der dominante Grund ist
später veränderte sensorische Speicherung; reine transiente Störung ist selten.

**D. Welche Rolle spielt inhibitorische Konkurrenz?** Sie ist Teil des
natürlichen Verlaufs, aber isolierte inhibitorische Rücknahmen reparieren keinen
der 72 detaillierten Endzustände.

**E. Welche Gewichtsänderungen verursachen Verluste?** Sensorische Cue-zu-
Entscheidungsgewichte: gemeinsame Rücknahme repariert 57/72 Fälle; aktuelle
Gewichte zerstören 60/72 frühere gemeinsame Fähigkeiten.

**F. Welche Rolle spielt frühe Exploration?** Unabhängige Fortsetzungen vom
identischen Zustand ändern die Robustheit deutlich (42 bis 50 robuste Netze von
72). Erfahrung ist kausal, aber kein einzelner Strom garantiert Erfolg.

**G. Welche Fehler sind kombiniert?** Drei der 72 Detailfälle erfordern eine
Faktorkombination; zwölf bleiben mit den präregistrierten Kopien offen.

**H. Wie stark hängt S4-d16 vom Abstand 16 ab?** Stark. Bei 12 Einheiten ist S4
signifikant schlechter, bei 32 signifikant besser. 16, 20 und 24 liefern
identische Resultate.

**I. Bleibt der Vorteil unregelmäßig und kontinuierlich erhalten?** Die Zahl
robuster Netze steigt (163 statt 68 beziehungsweise 35 statt 18), aber der
mittlere gemeinsame E1-Vorteil ist nicht statistisch abgesichert.

**J. Was geschieht an der Zeitgrenze?** Bei `delay−1` integriert der Eingang;
bei `delay` wird S4 deterministisch zuerst verarbeitet; bei `delay+1` ist der
alte Zustand bereits entfernt.

**K. Welche Netze sind knapp oder grundsätzlich gescheitert?** N3 und N4
treten nicht auf. Die Restpopulation ist daher überwiegend knapp und
beziehungsspezifisch, nicht grundsätzlich lernunfähig.

**L. Ist ein zusätzlicher Mechanismus notwendig?** Noch nicht belegt. Die
Fehler sind größtenteils mit vorhandener sensorischer Plastizität erklärbar.

**M. Gibt es einen Ansatzpunkt für B.5.7?** Ja: lokal beobachtbare negative
sensorische Gewichtsänderungen nach gemeinsamer Fähigkeit. B.5.7 soll deren
Eligibility- und Modulationsursprung untersuchen, ohne vorab Gewichte zu
schützen.

**N. Ist der Stand mergefähig?** Ja. Er ist reproduzierbar, diagnostisch
isoliert, vollständig getestet und verändert den normalen Lernpfad nicht.

## Wissenschaftliche Entscheidung

H1 (unvollständiger Erwerb) wird für die bestätigte Population verworfen. H2
(sensorische Konkurrenz bzw. Speicherung) wird stark gestützt. H3
(inhibitorische Überlagerung) und H4 (transiente Endzustandsstörung) erklären
die Population nicht isoliert. H5 und H6 werden bestätigt: exakte Zeitgrenzen
und das Episodenraster beeinflussen S4 wesentlich. H7 wird bestätigt, weil
alternative natürliche Erfahrungsfolgen unterschiedliche Fähigkeiten erzeugen.
H8 tritt in drei kombinierten Detailfällen auf.

S4 bleibt experimentell und standardmäßig aus. Aus dem positiven Befund wird
weder eine Bestwertsuche noch eine neue Schutz-, Wiederherstellungs- oder
Gedächtnisregel abgeleitet.

## Merge-Empfehlung

**Merge von B.5.6 nach `main`: Ja.**

Die Empfehlung gilt für Runner, Tests, Rohdaten und Dokumentation. Sie ist
keine Freigabe von S4 oder einer neuen neuronalen Produktionsfunktion.
