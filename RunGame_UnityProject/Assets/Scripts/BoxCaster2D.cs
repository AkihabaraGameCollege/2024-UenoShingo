using UnityEngine;

namespace RunGame
{
    // 指定した判定対象のコライダーと交差しているかを判定する機能を提供します。
    public class BoxCaster2D : MonoBehaviour
    {
        // ボックスの中心点を指定します。
        [SerializeField]
        [Tooltip("ボックスの中心点を指定します。")]
        private Vector2 center;
        // ボックスの幅と高さを指定します。
        [SerializeField]
        [Tooltip("ボックスの幅と高さを指定します。")]
        private Vector2 size;
        // 判定対象のレイヤーを指定します。
        [SerializeField]
        [Tooltip("判定対象のレイヤーを指定します。")]
        private LayerMask targetLayers = default;

        // 判定対象と交差しているかどうかを取得します。
        public bool IsCasted { get => isCasted; private set => isCasted = value; }
        private bool isCasted = false;

        // 固定フレームレートで呼び出される更新処理です。
        void FixedUpdate()
        {
            // 交差判定用のポイントを設定
            var centerPoint = transform.TransformPoint(center);
            // 交差を判定
            IsCasted = Physics2D.OverlapBox(centerPoint, size, transform.eulerAngles.z, targetLayers);
        }

        // Unityエディター上で常時描画するギズモを記述します。
        private void OnDrawGizmos()
        {
            // 交差判定用のポイントを設定
            var halfSize = size / 2;
            var centerPoints = new Vector3[] {
            center + new Vector2(-halfSize.x, +halfSize.y),
            center + new Vector2(+halfSize.x, +halfSize.y),
            center + new Vector2(+halfSize.x, -halfSize.y),
            center + new Vector2(-halfSize.x, -halfSize.y),
        };
            transform.TransformPoints(centerPoints);
            // 判定ラインを描画
            Gizmos.color = Color.yellow;
            Gizmos.DrawLineStrip(centerPoints, true);
        }
    }
}