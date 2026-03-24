<script lang="ts">
  const firmwareInfo = [
    { label: 'Firmware versie', value: 'Niet beschikbaar' },
    { label: 'Laatst gezien', value: 'Nooit' },
    { label: 'WiFi signaal', value: '\u2014' },
    { label: 'Batterij', value: '\u2014' }
  ];
</script>

<div class="rounded-xl bg-surface-2 p-6">
  <h2 class="font-display text-lg text-on-bg">Firmware</h2>

  <div class="mt-4 grid grid-cols-2 gap-4">
    {#each firmwareInfo as item}
      <div>
        <p class="font-label text-xs uppercase tracking-wider text-on-bg-muted">{item.label}</p>
        <p class="mt-1 font-body text-sm text-on-bg-dim">{item.value}</p>
      </div>
    {/each}
  </div>
</div>
