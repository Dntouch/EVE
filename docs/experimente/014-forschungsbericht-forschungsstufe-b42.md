# Forschungsbericht – Forschungsstufe B.4.2

## A. Ausgangslage

B.4.1 hatte eine kausale Wirkung gelernter Gewichte nachgewiesen, zugleich
aber einen Zusammenbruch der freien Handlungsfähigkeit beobachtet. B.4.2 prüft
deshalb vor einer neuen Lernregel drei Hypothesen: negative Modulation zerstört
angeborene Explorationswege, ein allgemeiner Schutz dieser Wege erhält
Alternativen, und die erhaltenen Alternativen ermöglichen anschließend Lernen.

Alle Varianten verwenden explizite Simulationszeit, getrennte Zufallsströme,
64 vorab festgelegte Seeds und eine unabhängige Auswertungsphase ohne neue
Gewichtsänderung. Fehlende und doppelte Ausgaben bleiben in der Statistik.
Der unveränderte B.4.1-Runner reproduziert unter dem neuen Build seine
`summary.csv` und `trials.csv` weiterhin bytegenau.

## B. Kausaldiagnose

In A0 waren anfangs je zwei Explorationswege zu A und B funktionsfähig. Nach
128 Erfahrungen waren bei fast allen Seeds alle vier Wege unter die
Feuergrenze gefallen; nach 512 Erfahrungen waren sie bei allen 64 Seeds
vollständig unwirksam. Im Mittel waren vier von vier Explorationssynapsen
abgeschwächt und vollständig auf Gewicht null gefallen.

Für die Ablation wurde nach 512 ausschließlich eine diagnostische Zustandskopie
verändert. Das Wiederherstellen der vier Explorationsgewichte auf ihren
ursprünglichen Wert stellte A und B in 64/64 Fällen wieder her. Keine andere
Synapse wurde verändert. Damit ist der in B.4.1 vermutete Kausalzusammenhang
zwischen negativer Modulation und Handlungsverlust isoliert nachgewiesen.

## C. Angeborene Exploration

A1 markiert nur die symmetrischen, angeborenen Explorationssynapsen als
plastizitätsgeschützt. A2 führt dieselbe Funktion über getrennte Relais. Der
Schutz enthält weder Reiz- noch Zielwissen und kann für K24 abgeschaltet
werden. Beide Varianten behielten nach 512 Erfahrungen je zwei Wege zu A und B.
A3 war laut Auftrag nur bei unzureichender Erreichbarkeit von A1/A2 optional;
da beide 64/64 Seeds vollständig erhielten, wurde A3 nicht nachträglich ergänzt.

| Variante | Bewertung | funktionsfähige Wege | fehlende Ausgaben |
|---|---:|---:|---:|
| A0, ungeschützt | −0,726074 | 0,984375 | 2.765 |
| A1, geschützt | −0,221191 | 4 | 443 |
| A2, getrennt und geschützt | −0,221191 | 4 | 443 |

Der gepaarte Vorteil von A1 gegen A0 beträgt +0,504883 mit einem
95-%-Bootstrapintervall von [0,402832; 0,608887]. Erhaltene Exploration ist
damit kausal nützlich, löst die Zwei-Reiz-Aufgabe allein aber noch nicht.

## D. Negative Modulation

Negative Modulation allein zerstörte die ungeschützten Wege und endete bei
−0,999023. Positive Modulation allein hielt mit geschützter Exploration alle
Wege offen und erreichte −0,111816. Ternäre Modulation erreichte −0,221191;
positive Modulation allein war gepaart um 0,109375 besser, mit einem
95-%-Intervall von [0,025391; 0,189453].

Wurde nur ein reproduzierbarer Anteil negativer Bewertungen moduliert, ergaben
sich −0,258789 bei 25 %, −0,307617 bei 50 % und −0,197754 bei 75 %. Gegenüber
voller negativer Modulation waren die gepaarten Unterschiede −0,037598,
−0,086426 und +0,023438. Nur der 50-%-Nachteil war ungleich null; eine
monotone Verbesserung durch bloßes Ausdünnen entstand nicht.

N5 bildet eine mittlere negative Schrittstärke von 0,5 ohne
Fließkommagewichte ab: Der unveränderte ganzzahlige Schritt eins wird auf jedes
zweite negative Ereignis angewandt. Gewichte und lokale Lernregel bleiben
unverändert. N5 erreichte −0,240234 und unterschied sich nicht belastbar von
voller ternärer Modulation.

## E. Exploration nach Rückmeldung

Ein allgemeines Explorationsfenster X1 verschlechterte die Bewertung auf
−0,440918, weil es 1.733 doppelte Ausgaben erzeugte. X2 löste nach negativer
Rückmeldung 2.640 zusätzliche, nicht rückwirkend gewertete Explorationen aus
und erreichte −0,213867. X3 erhöhte die Exploration der folgenden Episode und
erreichte −0,238281. Doppelte Ausgaben wurden neutral moduliert; fehlende
Handlungen wurden zusätzlich mit und ohne negative Rückmeldung kontrolliert.
Keine Variante wählte technisch den alternativen oder
richtigen Motorweg aus.

Fortgesetzte Exploration ist daher notwendig, aber ihre bloße Häufigkeit ist
kein Ersatz für eine reizabhängige Präferenz.

## F. Einzelreiz-Lernen

Die frühere K15-Auffälligkeit wurde mit identischen No-Learning-Kontrollen und
beiden Zielrichtungen wiederholt.

| Umweltziel | ohne Plastizität | geschützt, ternär | gepaarter Vorteil | 95-%-Intervall |
|---|---:|---:|---:|---:|
| A | −0,383789 | +1,000000 | +1,383789 | [1,354004; 1,413086] |
| B | −0,372559 | +1,000000 | +1,372559 | [1,341309; 1,401855] |

Alle 64 Seeds erreichten in der Auswertung ausschließlich das jeweils von der
Umwelt bevorzugte Ziel. Zufällige Rückmeldung blieb deutlich schwächer;
vertauschte Rückmeldung erlernte zuverlässig die nachteilige Alternative. Ohne
zusätzliche Exploration entstand keine Handlung. Das Ergebnis ist damit weder
eine Ausgangspräferenz noch ein fest verdrahtetes Ziel.
Positive Modulation allein erreichte im Einzelreizfall ebenfalls +1; negative
Modulation war für diesen Aufbau nicht erforderlich, blieb aber für die
anschließende Umweltumkehr wirksam.

Für eine einzige wiederholte sensorische Situation ist
erfahrungsabhängiges Lernen erstmals belastbar nachgewiesen.

## G. Umweltumkehr

Bei A→B lag die mittlere Belohnung im ersten 16-Episoden-Block bei 0,642578
und danach zunächst bei +1. Direkt nach der Umkehr sank sie auf 0,277344,
stieg im nächsten Block auf 0,998047 und anschließend wieder auf +1. Für B→A
entstand derselbe Verlauf. Die unabhängige Auswertung endete für beide
Richtungen bei +1.

Die alte Präferenz wurde ohne Zustandsreset abgeschwächt, während die zuvor
weniger bevorzugte Alternative durch geschützte Exploration erreichbar blieb
und aufgebaut wurde. Damit ist auch Umkehrlernen im Einzelreizfall belegt.

## H. Zwei-Reiz-Lernen

Die geschützte Zwei-Reiz-Variante erreichte −0,221191 gegenüber −0,384277 in
K0. Der gepaarte Vorteil +0,163086 ist mit [0,111328; 0,221680] belastbar.
Dennoch blieb die absolute Bewertung negativ. Bei Reiz 0 traten A und B 699
beziehungsweise 659 Mal auf; bei Reiz 1 waren es 635 und 896 Mal. Eine klare
reizabhängige Trennung entstand nicht. Die umgekehrte Umweltzuordnung zeigte
dasselbe Muster.

Auch nach 2.048 Erfahrungen blieb die mittlere Bewertung bei −0,252441. Der
Schutz verhindert den Zusammenbruch, erzeugt aber noch keine stabile
gleichzeitige Zuordnung zweier Reize zu zwei Handlungen.

## I. Lokalität der Plastizität

Geschützte Explorationssynapsen nehmen weiterhin regulär an neuronaler
Aktivität und lokaler Lernspurbildung teil, werden bei Modulation jedoch nicht
verändert. Ungeschützte Sensor→Motor-Synapsen können von null auf eins und bis
zur Feuerwirksamkeit wachsen oder wieder abgeschwächt werden. Der Schutz ist
eine gespeicherte Eigenschaft der angeborenen Verbindung und kein dynamisch
gewählter Lernkandidat.

Im Einzelreizversuch genügten im Mittel wenige positive Änderungen, um den
passenden Sensorweg dauerhaft wirksam zu machen. In der Zwei-Reiz-Aufgabe
wechselten Verstärkung und Abschwächung häufiger; die lokale G3-Zuordnung
stabilisierte die beiden konkurrierenden Reiz-Handlungs-Paare nicht zugleich.

## J. Stabilität

Gewichte blieben in [0,3], Eingangsbudgets wurden eingehalten und es gab keine
technischen Abbrüche. Die maximale Ereigniswarteschlange betrug 19. Geschützte
Wege blieben über 2.048 Erfahrungen erhalten. Ungeschützte A0-Wege endeten
dagegen ab 512 Erfahrungen bei allen Seeds vollständig unwirksam.

Persistenzformat 11 speichert den Schutz pro Synapse. Zustände mit
R2F-Erholung, G3, Eligibility, Rückmeldungsroute und ausstehender Modulation
wurden gespeichert, geladen und deterministisch fortgesetzt. Ältere Formate
bleiben lesbar und behandeln Synapsen erwartungsgemäß als ungeschützt.

## K. Ressourcen

Bei 64 Neuronen wurden 23 Synapsen, davon vier geschützt, und 45.840 Byte
gemessen. Bei 256 Neuronen waren es 1.072 Synapsen, vier geschützt, und
230.832 Byte. Das Schutzbit vergrößert die aktuelle `Synapse`-Struktur wegen
Speicherausrichtung um vier Byte je Synapse.

Die linearen Abschätzungen von 8,9 MB, 89 MB und 890 MB für 10.000, 100.000 und
eine Million Neuronen sind keine Messwerte. Der gesamte Lauf umfasste mehr als
eine Million Episoden; maschinenabhängige CPU-, RAM- und Ereigniswerte stehen
in `performance.csv`.

Die entscheidenden 256-Neuronen-Wiederholungen bestätigten das Muster:
Einzelreiz-Lernen erreichte +0,9375 und alle vier geschützten Wege blieben
erhalten; die Zwei-Reiz-Aufgabe blieb mit −0,235840 ungelöst.

## L. Abschlussbewertung

B.4.2 beantwortet die zentrale Forschungsfrage teilweise positiv.

1. Negative Modulation verursacht den Verlust angeborener Explorationswege.
2. Ein allgemeiner, symmetrischer Schutz verhindert diesen Verlust.
3. Mit erhaltener Exploration lernt EVE eine einzelne Reiz-Handlungs-Präferenz
   belastbar und kann sie nach Umweltumkehr wechseln.
4. Bei zwei Reizen entsteht zwar ein Vorteil zur Nullkontrolle, aber noch keine
   erfolgreiche reizabhängige Zuordnung.

Erfahrungsabhängiges Lernen ist damit erstmals für den eng begrenzten
Einzelreizfall nachgewiesen, nicht für allgemeines Verhalten. Der verbleibende
Engpass ist nicht mehr die Verfügbarkeit von Handlungsalternativen, sondern die
stabile lokale Trennung mehrerer konkurrierender Reiz-Handlungs-Zuordnungen.

Alle 303 automatisierten Tests bestehen. Die 25 verbindlichen Kontrollen, 64
Seeds, Rohversuche, Gewichtsverläufe, Langläufe, Kausalablationen und
Ressourcenmessungen liegen unter `research/results/b42/`.
