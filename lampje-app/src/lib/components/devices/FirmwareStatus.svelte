<script lang="ts">
  import type { Device } from '$lib/types';
  import { formatRelativeTime, formatWifiRssi } from '$lib/utils/formatters';

  let { device }: { device: Device } = $props();

  const firmwareInfo = $derived([
    {
      label: 'Firmware versie',
      value: device.firmwareVersion ?? 'Niet beschikbaar'
    },
    {
      label: 'Laatst gezien',
      value: device.lastHeartbeat
        ? formatRelativeTime(new Date(device.lastHeartbeat))
        : 'Nooit'
    },
    {
      label: 'WiFi signaal',
      value: formatWifiRssi(device.wifiRssi)
    }
  ]);
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
