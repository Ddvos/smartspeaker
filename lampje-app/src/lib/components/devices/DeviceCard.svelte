<script lang="ts">
  import type { Device } from '$lib/types';

  let { device }: { device: Device } = $props();

  let deviceName = $state(device.name);
  $effect(() => { deviceName = device.name; });
  let saving = $state(false);

  const statusColor = $derived(
    device.status === 'online'
      ? 'bg-green-500'
      : device.status === 'pairing'
        ? 'bg-amber-500'
        : 'bg-on-bg-muted'
  );

  const statusLabel = $derived(
    device.status === 'online'
      ? 'Online'
      : device.status === 'pairing'
        ? 'Wacht op koppeling'
        : 'Offline'
  );

  async function saveName() {
    if (deviceName === device.name) return;
    saving = true;
    await fetch(`/api/devices/${device.id}/config`, {
      method: 'PUT',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ name: deviceName })
    });
    saving = false;
  }
</script>

<div class="rounded-xl bg-surface-2 p-6">
  <h2 class="font-display text-lg text-on-bg">Verbind je Lampje</h2>
  <p class="mt-2 font-body text-sm text-on-bg-dim">
    Apparaat-ID: <span class="font-mono text-on-bg-muted">{device.chipId}</span>
  </p>

  <div class="mt-4 space-y-4">
    <div>
      <label for="device-name-{device.id}" class="mb-1.5 block font-label text-xs text-on-bg-muted">Apparaatnaam</label>
      <input
        id="device-name-{device.id}"
        type="text"
        class="w-full rounded-xl bg-surface-0 px-4 py-2.5 font-body text-sm text-on-bg placeholder:text-on-bg-muted focus:outline-none focus:ring-2 focus:ring-primary/20"
        placeholder="Bijv. Lampje Woonkamer"
        bind:value={deviceName}
        onblur={saveName}
      />
    </div>
  </div>

  <div class="mt-4 flex items-center gap-2">
    <span class="h-2 w-2 rounded-full {statusColor}"></span>
    <span class="font-label text-xs text-on-bg-muted">{statusLabel}</span>
    {#if saving}
      <span class="font-label text-xs text-on-bg-muted">&middot; Opslaan...</span>
    {/if}
  </div>
</div>
