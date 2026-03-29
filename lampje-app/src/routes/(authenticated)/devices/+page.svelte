<script lang="ts">
  import { page } from '$app/state';
  import { deviceStore } from '$lib/stores/devices.svelte';
  import DeviceCard from '$lib/components/devices/DeviceCard.svelte';
  import HardwareConfig from '$lib/components/devices/HardwareConfig.svelte';
  import FirmwareStatus from '$lib/components/devices/FirmwareStatus.svelte';
  import PairingDialog from '$lib/components/devices/PairingDialog.svelte';
  import type { Device } from '$lib/types';

  const data = $derived(page.data as { devices: Device[] });

  // Initialize store with SSR data
  $effect(() => {
    deviceStore.setDevices(data.devices);
  });

  const devices = $derived(deviceStore.devices);
</script>

<div class="mx-auto max-w-[800px] space-y-8">
  <h1 class="font-display text-3xl text-on-bg">Apparaten</h1>

  {#if devices.length === 0}
    <PairingDialog />
  {:else}
    {#each devices as device (device.id)}
      <DeviceCard {device} />
      <HardwareConfig config={device.config} deviceId={device.id} />
      <FirmwareStatus {device} />
    {/each}

    <div class="rounded-xl border-2 border-dashed border-surface-bright p-6 text-center">
      <p class="font-body text-sm text-on-bg-muted">Nog een Lampje toevoegen?</p>
      <PairingDialog />
    </div>
  {/if}
</div>
