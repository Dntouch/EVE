# Experimente

**Grundlage:** Abschnitte 19 und 23 des EVE-Konzepts

Das Konzept fordert reproduzierbare Versuche, mit denen sich tatsächliches
Lernen von fest programmierten Lösungswegen, zufälligen Treffern und verdeckten
LLM-Entscheidungen unterscheiden lässt.

Als zu speichernde Bestandteile nennt das Konzept:

- Ausgangszustand
- zeitliche Abfolge externer Ereignisse
- relevante Zufallszustände
- getroffene Entscheidungen
- Veränderungen des neuronalen Systems

Die [Experimentvorlage](VORLAGE.md) ist ein Arbeitsmittel. Ihre Felder sind keine
bereits beschlossene Versuchsmethodik und können für den jeweiligen Versuch
angepasst werden.

## Jüngste Forschungsstufe

- [Auftrag B.4](012-entwicklungsauftrag-forschungsstufe-b4.md)
- [Bericht B.4](012-forschungsbericht-forschungsstufe-b4.md)
- [Auftrag B.4.1](013-entwicklungsauftrag-forschungsstufe-b41.md)
- [Bericht B.4.1](013-forschungsbericht-forschungsstufe-b41.md)
- [Auftrag B.4.2](014-entwicklungsauftrag-forschungsstufe-b42.md)
- [Bericht B.4.2](014-forschungsbericht-forschungsstufe-b42.md)
- [Auftrag B.4.3](015-entwicklungsauftrag-forschungsstufe-b43.md)
- [Technische Analyse B.4.3](015-technische-analyse-g3-eligibility.md)
- [Bericht B.4.3](015-forschungsbericht-forschungsstufe-b43.md)
- [Auftrag B.4.4](016-entwicklungsauftrag-forschungsstufe-b44.md)
- [Technische Analyse B.4.4](016-technische-analyse-synapsenwachstum.md)
- [Bericht B.4.4](016-forschungsbericht-forschungsstufe-b44.md)
- [Auftrag B.4.5](017-entwicklungsauftrag-forschungsstufe-b45.md)
- [Technische Analyse B.4.5](017-technische-analyse-entscheidungsbindung.md)
- [Bericht B.4.5](017-forschungsbericht-forschungsstufe-b45.md)
- [Auftrag B.4.6](018-entwicklungsauftrag-forschungsstufe-b46.md)
- [Technische Analyse B.4.6](018-technische-analyse-inhibitorische-synapsen.md)
- [Bericht B.4.6](018-forschungsbericht-forschungsstufe-b46.md)
- [Auftrag B.4.7](019-entwicklungsauftrag-forschungsstufe-b47.md)
- [Technische Analyse B.4.7](019-technische-analyse-neuronale-symmetrie.md)
- [Bericht B.4.7](019-forschungsbericht-forschungsstufe-b47.md)
- [Auftrag B.4.8](020-entwicklungsauftrag-forschungsstufe-b48.md)
- [Technische Analyse B.4.8](020-technische-analyse-fruehe-erfahrungsbindung.md)
- [Bericht B.4.8](020-forschungsbericht-forschungsstufe-b48.md)
- [Auftrag B.4.9](021-entwicklungsauftrag-forschungsstufe-b49.md)
- [Technische Analyse B.4.9](021-technische-analyse-lokale-fehlerkorrektur.md)
- [Bericht B.4.9](021-forschungsbericht-forschungsstufe-b49.md)
- [Auftrag B.5.0](022-entwicklungsauftrag-forschungsstufe-b50.md)
- [Technische Analyse B.5.0](022-technische-analyse-lokale-kreditzuordnung.md)
- [Bericht B.5.0](022-forschungsbericht-forschungsstufe-b50.md)

Der Bericht trennt technische Gewichtsänderung ausdrücklich vom nicht
erreichten Nachweis einer Verhaltensverbesserung. B.4.1 validiert Zeit,
Zufallsquellen, Exploration und motorische Kausalität des Versuchsaufbaus.
B.4.2 erhält angeborene Exploration allgemein und weist dadurch begrenztes
Einzelreiz- und Umkehrlernen nach; die Zwei-Reiz-Aufgabe bleibt offen.
B.4.3 weist die lokale Kandidatenvermischung und das synaptische Überschreiben
kausal nach. Kein untersuchtes lokales Fenster erreicht gleichzeitiges
Mehrfachlernen; die allgemeine Freigabe bleibt deshalb gesperrt.
B.4.4 zeigt, dass dynamisches Wachstum zwar viele Nullgewicht-Verbindungen,
aber keine funktionalen Lernwege erzeugt. `active_phase` reproduziert einen
starken Sequenzeffekt ohne Wachstum; gemischtes Lernen bleibt ungelöst.
B.4.5 schließt zeitliche Altspuren und bloße Kandidatenkapazität als
dominante Mischfehler aus. Eine gezielte Ambiguitätsintervention weist
motorische Konkurrenz kausal als nächsten Engpass nach, erreicht das robuste
Lernziel aber nicht.
B.4.6 weist lokale inhibitorische Wirkung und erfahrungsabhängige
Gewichtsänderung nach. Die beste Variante verbessert B.4.5, verfehlt jedoch
Robustheit und Zuordnungssymmetrie; inhibitorische Konkurrenz bleibt daher
experimentell.
B.4.7 weist nach, dass konsistente technische Rollenabbildungen exakt
invariant sind, entdeckt aber eine feste Kopplung der historischen binären
Zufallsströme. Nach deren isolierter Entkopplung bleiben normale und inverse
Aufgaben deutlich unter dem Robustheitsziel. Gelernte Pfade binden stark an
konkrete frühe Explorationsrollen; autonome und umzulernende Entscheidungen
bleiben unzureichend.
B.4.8 lokalisiert diese Bindung zeitlich: autonome Antworten entstehen schon
nach 4–8 Erfahrungen, bleiben aber nicht stabil. Falsche autonome Antworten
unterdrücken weitere Exploration. Eine minimale negative-rückmeldungsabhängige
Exploration erhöht Aktivität, liefert jedoch keinen reproduzierbaren autonomen
Lernvorteil und wird nicht freigegeben.
B.4.9 zeigt, dass negative Modulation einzelne eindeutige Fehlantworten lokal
verändert und konkurrierende Wege strukturell erreichbar bleiben. Hemmung und
zeitliche Nachwirkung erklären den Restfehler nicht. Positiv bewertete
Alternativen werden jedoch nicht zuverlässig unter Erhalt der zweiten
Reizbeziehung gespeichert; deshalb wird keine neue Fehlerkorrekturfunktion
eingeführt.
B.5.0 weist prospektiv nach, dass die vorhandene Eligibility den richtigen
Sensorpfad einer explorativ erfolgreichen Handlung erreicht. Die erste
Änderung von 0 auf 1 bleibt unter der Feuergrenze; der zweite Schritt auf 2
ist in 364 Kopien notwendig und hinreichend. Natürliche Wiederholung und
Stabilität im gemischten Strom bleiben ungelöst, daher entsteht keine neue
Kreditzuordnungsregel.
