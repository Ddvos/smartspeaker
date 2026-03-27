<script lang="ts">
  const shortcuts = [
    { label: 'Zoeken', keys: '⌘K' },
    { label: 'Instellingen', keys: '⌘,' },
    { label: 'Nieuw gesprek', keys: '⌘N' },
    { label: 'Spraak starten', keys: 'Spatie (lang indrukken)' },
    { label: 'Spraak stoppen', keys: 'Esc' }
  ];

  const faqs = [
    {
      question: 'Hoe verbind ik Lampje met mijn account?',
      answer:
        'Ga naar Instellingen en voer je API-sleutels in. Je hebt minimaal een Google Gemini of OpenAI sleutel nodig. Klik op "Test" om te controleren of de verbinding werkt.'
    },
    {
      question: 'Welke talen ondersteunt Lampje?',
      answer:
        'Lampje is primair ontworpen voor het Nederlands, maar kan ook in het Engels communiceren. De spraakherkenning en tekst-naar-spraak werken het beste in het Nederlands.'
    },
    {
      question: 'Waar worden mijn gesprekken opgeslagen?',
      answer:
        'Alle gesprekken worden lokaal opgeslagen in een PostgreSQL-database. Er worden geen gegevens naar externe servers gestuurd, behalve de API-aanroepen naar je gekozen AI-model.'
    },
    {
      question: 'Kan ik mijn data exporteren?',
      answer:
        'Ja, via Instellingen > Data & Privacy kun je al je gesprekken en herinneringen exporteren als JSON-bestand. Je kunt ook selectief data verwijderen.'
    }
  ];
</script>

<div class="max-w-[640px] mx-auto space-y-8">
  <h1 class="font-display text-3xl text-on-bg">Help</h1>

  <section class="bg-surface-2 rounded-xl p-6 space-y-4">
    <h2 class="font-display text-lg text-on-bg">Over Lampje</h2>
    <p class="font-body text-sm text-on-bg-dim leading-relaxed">
      Lampje is een persoonlijke AI voice-assistent met geheugen. Via dit dashboard kun je gesprekken voeren,
      je kennisbank doorzoeken, en het apparaat configureren. Lampje onthoudt wat je vertelt en leert je
      voorkeuren kennen.
    </p>
  </section>

  <section class="bg-surface-2 rounded-xl p-6 space-y-4">
    <h2 class="font-display text-lg text-on-bg">Sneltoetsen</h2>
    <div class="space-y-2">
      {#each shortcuts as shortcut}
        <div class="flex justify-between items-center py-1.5">
          <span class="font-body text-sm text-on-bg-dim">{shortcut.label}</span>
          <kbd class="rounded-lg bg-surface-bright px-2.5 py-1 font-label text-xs text-on-bg-muted">
            {shortcut.keys}
          </kbd>
        </div>
      {/each}
    </div>
  </section>

  <section class="bg-surface-2 rounded-xl p-6 space-y-4">
    <h2 class="font-display text-lg text-on-bg">Veelgestelde vragen</h2>
    <div class="space-y-5">
      {#each faqs as faq}
        <div>
          <p class="font-body font-medium text-sm text-on-bg">{faq.question}</p>
          <p class="font-body text-sm text-on-bg-dim mt-1.5 leading-relaxed">{faq.answer}</p>
        </div>
      {/each}
    </div>
  </section>

  <section class="bg-surface-2 rounded-xl p-6 space-y-3">
    <h2 class="font-display text-lg text-on-bg">Versie</h2>
    <p class="font-label text-sm text-on-bg-muted">Lampje Dashboard v0.1.0</p>
    <p class="font-label text-sm text-on-bg-muted">SvelteKit &middot; Gemini &middot; PostgreSQL</p>
  </section>
</div>
