<script lang="ts">
  import DeviceCard from '$lib/components/devices/DeviceCard.svelte';
  import HardwareConfig from '$lib/components/devices/HardwareConfig.svelte';
  import FirmwareStatus from '$lib/components/devices/FirmwareStatus.svelte';
</script>

<div class="mx-auto max-w-[800px] space-y-8">
  <h1 class="font-display text-3xl text-on-bg">Apparaten</h1>
  <DeviceCard />
  <HardwareConfig />
  <FirmwareStatus />
</div>
