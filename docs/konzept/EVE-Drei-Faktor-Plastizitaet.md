# EVE – Lokale Drei-Faktor-Plastizität

## Zweck

Dieses Teilkonzept beschreibt den isolierten Forschungsmechanismus aus B.4.
Eine Synapse darf sich nur ändern, wenn drei lokal wirksame Bedingungen
zusammentreffen:

1. präsynaptische Aktivität,
2. postsynaptische Beteiligung als begrenzte Spur,
3. ein später eintreffender neuronaler Modulationsreiz.

Der Reiz sagt weder, welche Handlung richtig war, noch welche Synapse zu
ändern ist. Jeder Empfänger betrachtet ausschließlich seine eigenen wenigen
Spuren.

## Varianten und Grenzen

- E1 übernimmt die jüngsten G3-Eingänge, E2 markiert den Eingang einer
  tatsächlichen postsynaptischen Feuerentscheidung, E3 jede lokale
  Membranbeteiligung einschließlich Gewicht null.
- T0–T3 konfigurieren unmittelbare, konstante, gestufte oder
  aktivitätsabhängige Lebensdauern. Alle Zustände bleiben kapazitätsbegrenzt.
- M0–M3 erlauben keine, positive, negative oder ternäre Modulation.
- Verstärkung endet an Gewichtsmaximum und Eingangsbudget; Abschwächung endet
  bei null. Eine verbrauchte Spur wird standardmäßig entfernt.
- Von Rückmeldung erzeugte Spuren sind markiert und dürfen keine rekursive
  Selbstbestätigung auslösen.

Der Mechanismus ist ereignisgetrieben: Es gibt weder globale Synapsensuche noch
periodischen Tick und keine globale Handlungskennung.

Modulation wird seit B.4.1 über einen eigenen lokalen Rezeptorkontakt
verarbeitet. Sie ist ein neuronales Ereignis, aber kein erregender
Membraneingang: Sie darf weder G3 füllen noch selbst ein Motorfeuer auslösen.
Diese Trennung verhindert, dass Rückmeldungswege durch Plastizität unbemerkt zu
Handlungswegen werden.

## Geltungsbereich

Die Implementierung ist eine Forschungsoption und standardmäßig ausgeschaltet.
Sie ersetzt weder die unzureichende Handlungszuordnung aus B.3.1 noch belegt sie
Lernen. Entscheidend bleibt eine Verbesserung auf Auswertungsdurchläufen mit
deaktivierter Plastizität.
