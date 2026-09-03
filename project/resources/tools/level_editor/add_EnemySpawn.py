import bpy

class MYADDON_OT_add_enemy_spawn(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_enemy_spawn"
    bl_label = "EnemySpawn追加"
    bl_description = "EnemyPopEventにEnemySpawnを追加します"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        parent_obj = context.active_object
        if not parent_obj or parent_obj.get("type") != "EnemyPopEvent":
            self.report({'WARNING'}, "EnemyPopEventを選択してください")
            return {'CANCELLED'}

        # EnemySpawn作成
        bpy.ops.myaddon.myaddon_ot_spawn_create_symbol('EXEC_DEFAULT', type="Enemy")
        child_obj = context.active_object

        # 親子付け
        child_obj.parent = parent_obj
        # 位置をリセット (親の原点に配置)
        child_obj.location = (0, 0, 0)
        
        return {'FINISHED'}

class OBJECT_PT_enemy_spawn(bpy.types.Panel):
    bl_idname = "OBJECT_PT_enemy_spawn"
    bl_label = "Enemy Pop Event"
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "object"

    @classmethod
    def poll(cls, context):
        # EnemyPopEvent型の場合のみ表示
        return context.object and context.object.get("type") == "EnemyPopEvent"

    def draw(self, context):
        layout = self.layout
        layout.operator(MYADDON_OT_add_enemy_spawn.bl_idname)
