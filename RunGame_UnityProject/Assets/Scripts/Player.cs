using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// ユーザー入力を受け取ってプレイヤーを操作します。
public class Player : MonoBehaviour
{
    // 移動速度を指定します。
    [SerializeField]
    [Tooltip("移動速度を指定します。")]
    private float speed = 2;
    // ジャンプ力を指定します。
    [SerializeField]
    [Tooltip("ジャンプ力を指定します。")]
    private Vector2 jumpPower = new(0, 6);
    // 側面衝突時のノックバック力を指定します。
    [SerializeField]
    [Tooltip("側面衝突時のノックバック力を指定します。")]
    private Vector2 knockBackPower = new Vector2(-16, 4);
    // 地面との交差判定用のチェッカーを指定します。
    [SerializeField]
    [Tooltip("地面との交差判定用のチェッカーを指定します。")]
    private BoxCaster2D groundChecker = null;
    // 壁との交差判定用のチェッカーを指定します。
    [SerializeField]
    [Tooltip("壁との交差判定用のチェッカーを指定します。")]
    private BoxCaster2D wallChecker = null;

    // コンポーネントを参照しておく変数
    new Rigidbody2D rigidbody;

    // Start is called before the first frame update
    void Start()
    {
        // コンポーネントを事前に参照
        rigidbody = GetComponent<Rigidbody2D>();
    }

    // 毎フレームに一度実行される更新処理です。
    void Update()
    {
        // 接地状態の場合
        if (groundChecker.IsCasted)
        {
            // ジャンプ
            if (Input.GetButtonDown("Jump"))
            {
                rigidbody.AddForce(jumpPower, ForceMode2D.Impulse);
            }

            // 等速度運動
            var velocity = rigidbody.velocity;
            velocity.x = speed;
            rigidbody.velocity = velocity;
        }
    }

    // 固定フレームレートで呼び出される更新処理です。
    void FixedUpdate()
    {
        // 壁との衝突
        if (wallChecker.IsCasted)
        {
            // ノックバック
            rigidbody.AddForce(transform.TransformDirection(knockBackPower), ForceMode2D.Force);
        }
    }
}