<script lang="ts">
  import ApiKeysSection from '$lib/components/settings/ApiKeysSection.svelte';
  import ModelSelection from '$lib/components/settings/ModelSelection.svelte';
  import VoiceSelection from '$lib/components/settings/VoiceSelection.svelte';
  import PersonalitySlider from '$lib/components/settings/PersonalitySlider.svelte';
  import SystemPrompt from '$lib/components/settings/SystemPrompt.svelte';
  import DataPrivacy from '$lib/components/settings/DataPrivacy.svelte';
</script>

<div class="max-w-[720px] mx-auto space-y-10">
  <h1 class="font-display text-3xl text-on-bg">Instellingen</h1>

  <section class="bg-surface-2 rounded-xl p-6">
    <h2 class="font-display text-lg text-on-bg mb-5">API Sleutels</h2>
    <ApiKeysSection />
  </section>

  <section class="bg-surface-2 rounded-xl p-6">
    <ModelSelection />
  </section>

  <section class="bg-surface-2 rounded-xl p-6">
    <VoiceSelection />
  </section>

  <section class="bg-surface-2 rounded-xl p-6">
    <PersonalitySlider />
  </section>

  <section class="bg-surface-2 rounded-xl p-6">
    <SystemPrompt />
  </section>

  <section class="bg-surface-2 rounded-xl p-6">
    <DataPrivacy />
  </section>
</div>
