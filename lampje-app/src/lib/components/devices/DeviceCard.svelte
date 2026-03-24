<script lang="ts">
  let deviceId = $state('');
  let deviceName = $state('Lampje Proto-1');
</script>

<div class="rounded-xl bg-surface-2 p-6">
  <h2 class="font-display text-lg text-on-bg">Verbind je Lampje</h2>
  <p class="mt-2 font-body text-sm text-on-bg-dim">
    Het fysieke Lampje-apparaat wordt via WiFi verbonden. Voer het apparaat-ID in of wacht op automatische detectie.
  </p>

  <div class="mt-4 space-y-4">
    <div>
      <label for="device-id" class="mb-1.5 block font-label text-xs text-on-bg-muted">Apparaat-ID</label>
      <input
        id="device-id"
        type="text"
        class="w-full rounded-xl bg-surface-0 px-4 py-2.5 font-body text-sm text-on-bg placeholder:text-on-bg-muted focus:outline-none focus:ring-2 focus:ring-primary/20"
        placeholder="Bijv. LAMPJE-001"
        bind:value={deviceId}
      />
    </div>

    <div>
      <label for="device-name" class="mb-1.5 block font-label text-xs text-on-bg-muted">Apparaatnaam</label>
      <input
        id="device-name"
        type="text"
        class="w-full rounded-xl bg-surface-0 px-4 py-2.5 font-body text-sm text-on-bg placeholder:text-on-bg-muted focus:outline-none focus:ring-2 focus:ring-primary/20"
        placeholder="Bijv. Lampje Woonkamer"
        bind:value={deviceName}
      />
    </div>
  </div>

  <div class="mt-4 flex items-center gap-2">
    <span class="h-2 w-2 rounded-full bg-on-bg-muted"></span>
    <span class="font-label text-xs text-on-bg-muted">Nog niet verbonden</span>
  </div>
</div>
