<script lang="ts">
  import ConversationList from '$lib/components/conversations/ConversationList.svelte';
  import ConversationDetail from '$lib/components/conversations/ConversationDetail.svelte';
</script>

<div class="flex h-[calc(100vh-6rem)] gap-0">
  <!-- Left panel: conversation list -->
  <div class="flex w-[360px] shrink-0 flex-col overflow-hidden rounded-xl bg-surface-0/50 p-4">
    <ConversationList />
  </div>

  <!-- Right panel: conversation detail -->
  <div class="flex min-w-0 flex-1 flex-col overflow-hidden pl-6">
    <ConversationDetail />
  </div>
</div>
