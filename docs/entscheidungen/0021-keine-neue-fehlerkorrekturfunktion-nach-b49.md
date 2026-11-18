# Entscheidung 0021: Keine neue Fehlerkorrekturfunktion nach B.4.9

- Status: angenommen
- Datum: 18. November 2026
- Bezug: Entscheidung 0020

## Kontext

B.4.9 trennt E1 und E2 erstmals in unabhängige Kopien desselben vollständig
trainierten Zustands. 384/384 B.4.8-Referenzen reproduzieren E2 und sämtliche
Lerngewichte exakt. 2.688 Entdeckungs-, 1.152 Bestätigungs- und 2.048
Aufgabenläufe bestätigen, dass keine Gruppe die autonomen Kriterien erreicht.

In 122 kopierten autonomen Fehlentscheidungen wirkt negative Modulation lokal
und rechtzeitig. Kein Fall wiederholt unmittelbar dieselbe eindeutige falsche
Antwort; 74 wechseln zur richtigen Antwort, 45 bleiben still und drei werden
doppelt. Hemmungsablationen ändern diese Verteilung nicht, und alle 122
unterlegenen Wege sind strukturell aktivierbar.

Der verbleibende Engpass zeigt sich nach erfolgreicher Exploration. Nur 35/67
positiv bewertete Alternativen werden beim nächsten gleichen Reiz korrekt
autonom abgerufen; nur 21/67 erhalten gleichzeitig die zweite Beziehung.
Wiederholte negative Bewertung erhöht stattdessen die Zahl stiller Zustände.

## Entscheidung

K7, eine neue lokale Fehlerkorrekturfunktion, wird nicht implementiert. Damit
entfällt auch K8. Es gibt keinen isolierten Nachweis, dass ein zusätzlicher
Erregbarkeits-, Hemmungs- oder Bereitschaftszustand den beobachteten Fehler
verursacht oder behebt. Ein solcher Zustand wäre derzeit eine unbelegte neue
Hypothese.

Als gesichert gelten:

- negative Modulation verändert eine aktuelle autonome Fehlentscheidung
  kausal und eligibility-gebunden,
- der alternative Handlungsweg bleibt strukturell erreichbar,
- gelernte inhibitorische Gewichte und zeitliche Nachwirkung sind nicht der
  dominante Engpass,
- bloße weitere Abschwächung kann autonome Aktivität vollständig beseitigen,
- positive Alternativerfahrung wird nicht zuverlässig und interferenzarm an
  den auslösenden Reiz gebunden,
- E1-Retention und technische Persistenz speichern den Zustand exakt, lösen
  aber weder Mehrfachlernen noch Umweltumkehr.

Die allgemeine Lernfreigabe und die Freigabe adaptiver Exploration bleiben
gesperrt.

## Auftrag für B.5.0

B.5.0 soll vor jeder neuen Mechanik die lokale Kreditzuordnung einer positiv
bewerteten Alternativerfahrung isolieren. Zu prüfen ist insbesondere, welche
gleichzeitig aktiven Reiz-, Entscheidungs-, Explorations- und Hemmspuren durch
positive Modulation verstärkt werden, warum daraus häufig der falsche oder
kein autonomer Abruf entsteht und wie die zweite Reizbeziehung dabei
interferiert.

Eine neue Lern- oder Zustandsfunktion ist erst zulässig, wenn eine gezielte
Zustandskopie-Ablation einen einzelnen lokalen Engpass kausal ausweist. E1
bleibt primär; normale und inverse Rollen, mehrere unabhängige Stromfamilien,
No-Learning, Timing, Persistenz und Umweltumkehr bleiben Pflichtkontrollen.

