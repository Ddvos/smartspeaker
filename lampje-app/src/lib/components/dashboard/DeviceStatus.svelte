<script lang="ts">
  import type { Device } from '$lib/types';

  let { device = null }: { device: Device | null } = $props();

  const statusColor = $derived(
    !device
      ? 'bg-on-bg-muted/40'
      : device.status === 'online'
        ? 'bg-green-500'
        : device.status === 'pairing'
          ? 'bg-amber-500'
          : 'bg-on-bg-muted/40'
  );

  const statusLabel = $derived(
    !device
      ? 'Geen apparaat'
      : device.status === 'online'
        ? 'Online'
        : device.status === 'pairing'
          ? 'Wacht op koppeling'
          : 'Offline'
  );
</script>

<div class="bg-surface-2 rounded-xl p-5">
  <p class="font-label text-xs uppercase tracking-wider text-on-bg-muted">Apparaat</p>

  <div class="mt-3">
    <p class="font-body text-sm text-on-bg">{device?.name ?? 'Geen Lampje gekoppeld'}</p>
    <div class="flex items-center gap-2 mt-1">
      <span class="inline-block h-1.5 w-1.5 rounded-full {statusColor}"></span>
      <span class="text-on-bg-muted text-xs font-label">{statusLabel}</span>
    </div>

    {#if device}
      <div class="grid grid-cols-2 gap-3 mt-4">
        <div class="text-center">
          <p class="font-label text-[10px] uppercase text-on-bg-muted">Voice</p>
          <p class="font-body text-xs text-on-bg-dim mt-0.5">{device.config.voice || 'Niet ingesteld'}</p>
        </div>
        <div class="text-center">
          <p class="font-label text-[10px] uppercase text-on-bg-muted">STT</p>
          <p class="font-body text-xs text-on-bg-dim mt-0.5">{device.config.sttProvider}</p>
        </div>
        <div class="text-center">
          <p class="font-label text-[10px] uppercase text-on-bg-muted">TTS</p>
          <p class="font-body text-xs text-on-bg-dim mt-0.5">{device.config.ttsProvider}</p>
        </div>
        <div class="text-center">
          <p class="font-label text-[10px] uppercase text-on-bg-muted">Firmware</p>
          <p class="font-body text-xs text-on-bg-dim mt-0.5">{device.firmwareVersion ?? '—'}</p>
        </div>
      </div>
    {/if}

    <a
      href="/devices"
      class="mt-4 block w-full bg-surface-bright text-on-bg-dim rounded-xl py-2 text-sm text-center hover:bg-surface-1 font-body transition-colors duration-150"
    >
      {device ? 'Configureer' : 'Koppel apparaat'}
    </a>
  </div>
</div>
