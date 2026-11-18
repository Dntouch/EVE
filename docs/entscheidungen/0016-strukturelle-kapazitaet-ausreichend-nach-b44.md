# Entscheidung 0016: Strukturelle Kapazität für zwei Sequenzbeziehungen ausreichend

- Status: angenommen
- Datum: 18. November 2026
- Bezug: Entscheidungen 0014–0015

## Kontext

B.4.4 vergleicht die unveränderte 23-Synapsen-Referenz mit dynamischem
Wachstum, statischer Zusatzkapazität, lokaler `active_phase`-Auswahl und deren
Kombination. Dynamisches Wachstum erzeugt rund 104–112 neue Synapsen je
zentralem Lauf, aber keine davon wird funktional gewichtet. S1 ist exakt S0,
S4 exakt S3. Statische Zusatzkapazität bringt keinen belastbaren Vorteil.

`active_phase` ohne Wachstum erhält in beiden Lernreihenfolgen im Mittel beide
Beziehungen über 75 % und verbessert S0 um mehr als 15 Punkte mit positivem
Konfidenzintervall. Unabhängige Seeds reproduzieren diesen Sequenzeffekt. Im
alternierenden und gemischten Lernen bleibt die Leistung jedoch niedrig und
nur ungefähr die Hälfte der Einzelseeds besteht beide Schwellen zugleich.

## Entscheidung

Für zwei nacheinander trainierte Beziehungen wird Hypothese A angenommen:
Die vorhandene Struktur besitzt grundsätzlich ausreichende Kapazität; die
lokale Rückmeldungszuordnung ist der dominante Engpass. Das bestehende
ereignisgetriebene Synapsenwachstum wird nicht als Lernlösung freigegeben und
bleibt standardmäßig kontrolliert/deaktiviert.

`active_phase` bleibt eine Forschungsvariante. Wegen des Scheiterns bei
alternierenden und gemischten Erfahrungen wird sie nicht zum allgemeinen
Produktionsstandard und nicht als Nachweis allgemeinen Mehrfachlernens
klassifiziert.

## Folgen

- Es wird kein zielgerichtetes Wachstum und keine globale Reiz-, Handlungs-
  oder Episodenkennung eingeführt.
- Eine größere Struktur ist ohne funktionale Gewichtung kein Lernerfolg.
- Künftige Versuche müssen Sequenz-, Alternierungs- und Mischaufgaben getrennt
  ausweisen und neben Gruppenmitteln den Anteil gleichzeitig erfolgreicher
  Seeds berichten.
- Die nächste Ursachenprüfung soll lokale zeitliche/Koinzidenzbindung bei
  verschachtelten Erfahrungen untersuchen.
- Die allgemeine Freigabesperre der Entscheidungen 0012–0015 bleibt bestehen.
